#include "Player.h"
#include "application/map/Map.h"
#include <Input.h>

void Player::Initialize(const Map& map) {
	// イヌ初期化
	dog_ = std::make_unique<Dog>();
	dog_->Initialize();

	// サル初期化
	monkey_ = std::make_unique<Monkey>();
	monkey_->Initialize();

	//// スポーン（グリッド）を取得
	//if (auto p = map.GetDogSpawnWorld()) { dog_->SetWorldPosition(*p); }
	//if (auto p = map.GetMonkeySpawnWorld()) { monkey_->SetWorldPosition(*p); }

	// スポーンはグリッドで受け取って内部状態にセット
	if (auto g = map.GetDogSpawnGrid())    dogGrid_ = *g;
	if (auto g = map.GetMonkeySpawnGrid()) monkeyGrid_ = *g;

	// ワールドへスナップ
	SnapToWorld(Active::Dog, map);
	SnapToWorld(Active::Monkey, map);
}


void Player::Update(Map& map) {
	Move(map);
	RedoUndo(map);

	dog_->Update();
	monkey_->Update();
}

void Player::Move(Map& map) {
	// 操作対象の切替（必要なら）
	if (Input::GetInstance()->TriggerKey(DIK_1)) active_ = Active::Dog;
	if (Input::GetInstance()->TriggerKey(DIK_2)) active_ = Active::Monkey;

	// 押下1回 → 1マス
	int dx = 0, dy = 0;
	auto in = Input::GetInstance();

	// 同時押しが来たときは最初に検知した方向を優先（if-elseの順序で制御）
	if (in->TriggerKey(DIK_W) || in->TriggerKey(DIK_UP)) { dy = -1; }
	else if (in->TriggerKey(DIK_S) || in->TriggerKey(DIK_DOWN)) { dy = 1; }
	else if (in->TriggerKey(DIK_A) || in->TriggerKey(DIK_LEFT)) { dx = -1; }
	else if (in->TriggerKey(DIK_D) || in->TriggerKey(DIK_RIGHT)) { dx = 1; }

	if (dx != 0 || dy != 0) {
		TryStep(active_, dx, dy, map);
	}
}

void Player::RedoUndo(Map& map) {
	auto in = Input::GetInstance();
	Vector3 origin_ = { 0,0,0 };
	float tileSize_ = 2.0f;

	auto tileToWorld = [&](int x, int y, float yOffset = 0.0f) {
		// Yを反転してワールド座標に
		int flippedY = (map.GetRedoUndoSystem()->reflectionMapState().height - 1) - y;
		return Vector3{
			origin_.x + x * tileSize_,
			origin_.y + yOffset,
			origin_.z + flippedY * tileSize_
		};
		};

	// 共通の復元処理
	auto restore = [&](const auto& csvMapData_) {
		map.blocksL1_.clear();
		map.blocksL2_.clear();

		// --- Layer1（犬のフロア） ---
		for (int y = 0; y < csvMapData_.height; ++y) {
			for (int x = 0; x < csvMapData_.width; ++x) {
				MapChipType t = csvMapData_.layer1[y][x];
				if (!map.IsRenderable(t)) continue;

				auto pos = tileToWorld(x, y, 0.0f);
				auto blk = std::make_unique<Block>();
				blk->Initialize(t, pos);
				map.blockScaleY_ = blk->GetScaleY();
				map.blocksL1_.push_back(std::move(blk));
			}
		}

		// --- Layer2（猿のフロア） ---
		for (int y = 0; y < csvMapData_.height; ++y) {
			for (int x = 0; x < csvMapData_.width; ++x) {
				MapChipType t = MapChipType::Empty;
				if (y < (int)csvMapData_.layer2.size() && x < (int)csvMapData_.layer2[y].size()) {
					t = csvMapData_.layer2[y][x];
				}
				if (!map.IsRenderable(t)) continue;

				auto pos = tileToWorld(x, y, tileSize_);

				auto blk = std::make_unique<Block>();
				blk->Initialize(t, pos);
				if (t == MapChipType::BlockMonkey) {
					map.l2BlockAt_[y][x] = blk.get();
				}
				map.blocksL2_.push_back(std::move(blk));
			}
		}

		// 犬・猿の位置を反映
		dogGrid_ = { (int)csvMapData_.spawnDog.x,(int)csvMapData_.spawnDog.y };
		SnapToWorld(Active::Dog, map);

		monkeyGrid_ = { (int)csvMapData_.spawnMonkey.x,(int)csvMapData_.spawnMonkey.y };
		SnapToWorld(Active::Monkey, map);
		};

	if (in->TriggerKey(DIK_Z)) {
		if (map.GetRedoUndoSystem()->Redo()) {
			restore(map.GetRedoUndoSystem()->reflectionMapState());
		}
	}
	else if (in->TriggerKey(DIK_Y)) {
		if (map.GetRedoUndoSystem()->Undo()) {
			restore(map.GetRedoUndoSystem()->reflectionMapState());
		}
	}
}


void Player::TryStep(Active who, int dx, int dy, Map& map) {
	GridPos& g = (who == Active::Dog) ? dogGrid_ : monkeyGrid_;
	const int nx = g.x + dx;
	const int ny = g.y + dy;

	if (who == Active::Dog) {
		// 先に「次マスに箱ある？」を明確に判定
		if (map.HasBlockMonkeyAt(nx, ny)) {
			// 押せたら犬も進む。押せなければ何もしない（＝めり込まない）
			if (map.TryPushBlockByDog(g.x, g.y, dx, dy, monkeyGrid_)) {
				g.x = nx; g.y = ny;
				SnapToWorld(who, map);
			}
			return; // ここで終了。通常歩行へは進まない
		}

		// 箱が無いなら通常歩行
		if (map.IsWalkableFor(ActorKind::Dog, nx, ny)) {
			g.x = nx; g.y = ny;
			SnapToWorld(who, map);
		}
		return;
	}

	// 猿は従来どおり
	if (map.IsWalkableFor(ActorKind::Monkey, nx, ny)) {
		g.x = nx; g.y = ny;
		SnapToWorld(who, map);
	}
}

void Player::SnapToWorld(Active who, const Map& map) {
	GridPos& g = (who == Active::Dog) ? dogGrid_ : monkeyGrid_;
	ActorKind  k = (who == Active::Dog) ? ActorKind::Dog : ActorKind::Monkey;
	const Vector3 w = map.WorldFromGridFor(k, g.x, g.y);

	if (who == Active::Dog) dog_->SetWorldPosition(w);
	else                    monkey_->SetWorldPosition(w);
}
