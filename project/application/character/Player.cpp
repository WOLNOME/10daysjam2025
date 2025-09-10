#include "Player.h"
#include "application/map/Map.h"
#include <Input.h>
#include "MyMath.h"
#include <algorithm>

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
	// 1) 補間を進める
	UpdateTweens(kDeltaTime);

	// 2) ゴール上なら回転（補間中でも回す）
	if (map.IsGoalFor(ActorKind::Dog, dogGrid_.x, dogGrid_.y)) { dog_->AddYaw(-0.03f); }
	if (map.IsGoalFor(ActorKind::Monkey, monkeyGrid_.x, monkeyGrid_.y)) { monkey_->AddYaw(-0.03f); }

	// 3) どちらかが補間中なら入力は受け付けない
	if (dogTween_.active || monkeyTween_.active) {
		dog_->Update();
		monkey_->Update();
		return;
	}

	// 4) 補間していない時だけ入力処理
	Move(map);
	RedoUndo(map);

	// 5) 今フレームでゴールに入った直後にも回転が乗る
	if (map.IsGoalFor(ActorKind::Dog, dogGrid_.x, dogGrid_.y)) { dog_->AddYaw(-0.03f); }
	if (map.IsGoalFor(ActorKind::Monkey, monkeyGrid_.x, monkeyGrid_.y)) { monkey_->AddYaw(-0.03f); }

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

		//各レイヤーの初期化
		for (int y = 0; y < csvMapData_.height; ++y) {
			for (int x = 0; x < csvMapData_.width; ++x) {
				MapChipType t1 = csvMapData_.layer1[y][x];
				MapChipType t2 = csvMapData_.layer2[y][x];
				auto pos1 = tileToWorld(x, y, 0.0f);
				auto pos2 = tileToWorld(x, y, tileSize_);
				auto blk1 = std::make_unique<Block>();
				auto blk2 = std::make_unique<Block>();
				auto blkUp = std::make_unique<Block>();
				blk1->Initialize(t1, pos1);
				blk2->Initialize(t2, pos2);
				blkUp->Initialize(MapChipType::Empty, pos2);

				//ブーツブロック発見時
				bool isBootBlock = false;
				if (t1 == MapChipType::BootBlockOff) {
					// 参照テーブルに登録
					map.l1BootAt_[y][x] = blk1.get();
					// レイヤー2の場所のBlockを生成しておく
					map.l2BootAt_[y][x] = blkUp.get();
					// ブーツブロックがある
					isBootBlock = true;
				}
				//スイッチ発見時
				if (t2 == MapChipType::SwitchOff) {
					//参照テーブルに登録
					map.l2SwitchAt_[y][x] = blk2.get();
				}

				// 最初に見つけた“床”で一度だけ取得（以後は固定）
				if (!map.blockScaleCaptured_ &&
					(t1 == MapChipType::FloorDog)) {
					map.blockScaleY_ = blk1->GetScaleY();
					map.blockScaleCaptured_ = true;
				}
				map.blocksL1_.push_back(std::move(blk1));
				if (!isBootBlock) {
					map.blocksL2_.push_back(std::move(blk2));
				}
				else {
					map.blocksL2_.push_back(std::move(blkUp));
				}

			}
		}
		//blockScaleCaptured_をfalseに戻す
		map.blockScaleCaptured_ = false;

		// 犬・猿の位置を反映
		dogGrid_ = { (int)csvMapData_.spawnDog.x,(int)csvMapData_.spawnDog.y };
		SnapToWorld(Active::Dog, map);

		monkeyGrid_ = { (int)csvMapData_.spawnMonkey.x,(int)csvMapData_.spawnMonkey.y };
		SnapToWorld(Active::Monkey, map);
		};

	if (in->TriggerKey(DIK_Z) || in->TriggerPadButton(GamepadButton::LeftShoulder)) {
		if (map.GetRedoUndoSystem()->Redo()) {
			restore(map.GetRedoUndoSystem()->reflectionMapState());
			map.csvMapData_ = map.GetRedoUndoSystem()->reflectionMapState();
		}
	}
	else if (in->TriggerKey(DIK_Y) || in->TriggerPadButton(GamepadButton::RightShoulder)) {
		if (map.GetRedoUndoSystem()->Undo()) {
			restore(map.GetRedoUndoSystem()->reflectionMapState());
			map.csvMapData_ = map.GetRedoUndoSystem()->reflectionMapState();
		}
	}
}


void Player::TryStep(Active who, int dx, int dy, Map& map) {
	GridPos& g = (who == Active::Dog) ? dogGrid_ : monkeyGrid_;
	const int nx = g.x + dx;
	const int ny = g.y + dy;

	// 進行前のワールド座標（from）
	const ActorKind k = (who == Active::Dog) ? ActorKind::Dog : ActorKind::Monkey;
	const Vector3 from = map.WorldFromGridFor(k, g.x, g.y);

	if (who == Active::Dog) {
		// 先に箱がある？
		if (map.HasBlockMonkeyAt(nx, ny)) {
			// 押せたら前進（サルが乗っている・先がNGなら false）
			if (map.TryPushBlockByDog(g.x, g.y, dx, dy, monkeyGrid_)) {
				g.x = nx; g.y = ny;
				const Vector3 to = map.WorldFromGridFor(k, g.x, g.y);
				BeginTween(Active::Dog, from, to, moveDuration_);
			}
			return; // 押せなければ何もしない（めり込み防止）
		}

		// 通常歩行
		if (map.IsWalkableFor(ActorKind::Dog, nx, ny)) {
			g.x = nx; g.y = ny;
			const Vector3 to = map.WorldFromGridFor(k, g.x, g.y);
			BeginTween(Active::Dog, from, to, moveDuration_);

			map.OnPlayerStepped(ActorKind::Dog, g.x, g.y, dogGrid_, monkeyGrid_);

			//RedoUndoシステムに犬の移動を保存
			CsvMapData newState = map.GetRedoUndoSystem()->reflectionMapState();
			newState.spawnDog = { (float)g.x,(float)g.y };
			map.GetRedoUndoSystem()->AddNewHistory(newState);
		}
		return;
	}

	// 猿（箱は押さない）
	if (map.IsWalkableFor(ActorKind::Monkey, nx, ny)) {
		g.x = nx; g.y = ny;
		const Vector3 to = map.WorldFromGridFor(k, g.x, g.y);
		BeginTween(Active::Monkey, from, to, moveDuration_);

		map.OnPlayerStepped(ActorKind::Monkey, g.x, g.y, dogGrid_, monkeyGrid_);

		//RedoUndoシステムに猿の移動を保存
		CsvMapData newState = map.GetRedoUndoSystem()->reflectionMapState();
		newState.spawnMonkey = { (float)g.x,(float)g.y };
		map.GetRedoUndoSystem()->AddNewHistory(newState);
	}
}

void Player::SnapToWorld(Active who, const Map& map) {
	GridPos& g = (who == Active::Dog) ? dogGrid_ : monkeyGrid_;
	ActorKind  k = (who == Active::Dog) ? ActorKind::Dog : ActorKind::Monkey;
	const Vector3 w = map.WorldFromGridFor(k, g.x, g.y);

	if (who == Active::Dog) dog_->SetWorldPosition(w);
	else                    monkey_->SetWorldPosition(w);
}

void Player::BeginTween(Active who, const Vector3& from, const Vector3& to, float durationSec) {
	Tween& tw = (who == Active::Dog) ? dogTween_ : monkeyTween_;
	tw.from = from;
	tw.to = to;
	tw.t = 0.0f;
	tw.duration = (std::max)(0.001f, durationSec);
	tw.active = true;
}

void Player::UpdateTweens(float dtSec) {
	auto step = [&](Tween& tw, auto* actor) {
		if (!tw.active) return;
		tw.t += (dtSec / tw.duration);
		if (tw.t >= 1.0f) { tw.t = 1.0f; tw.active = false; }
		const float e = MyMath::EaseOutQuad(std::clamp(tw.t, 0.0f, 1.0f));
		const Vector3 p = MyMath::Lerp(tw.from, tw.to, e);
		actor->SetWorldPosition(p);
		};
	step(dogTween_, dog_.get());
	step(monkeyTween_, monkey_.get());
}
