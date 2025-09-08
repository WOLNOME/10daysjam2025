#include "Map.h"

void Map::Initialize(const std::string& filepath) {
	CsvLoader loader;
	loader.Initialize();
	csvMapData_ = loader.Loader(filepath);

	blocksL1_.clear();
	blocksL2_.clear();

	//RedoUndoシステムの初期化
	redoUndoSystem_ = std::make_unique<RedoUndoSystem>();
	redoUndoSystem_->Initialize(csvMapData_);

	// 初期化の前にテーブルをリサイズしておく（Initialize の冒頭あたり）
	l2BlockAt_.assign(
		csvMapData_.height,
		std::vector<Block*>(csvMapData_.width, nullptr)
	);

	auto tileToWorld = [&](int x, int y, float yOffset = 0.0f) {

		int flippedY = (csvMapData_.height - 1) - y;
		return Vector3{
			origin_.x + x * tileSize_,
			origin_.y + yOffset,
			origin_.z + flippedY * tileSize_
		};
		};

	// --- Layer1（犬のフロア） ---
	for (int y = 0; y < csvMapData_.height; ++y) {
		for (int x = 0; x < csvMapData_.width; ++x) {
			MapChipType t = csvMapData_.layer1[y][x];
			if (!IsRenderable(t)) continue;

			auto pos = tileToWorld(x, y, 0.0f);
			auto blk = std::make_unique<Block>();
			blk->Initialize(t, pos);
			blockScaleY_ = blk->GetScaleY();
			blocksL1_.push_back(std::move(blk));
		}
	}
	// --- Layer2（猿のフロア） ---
	for (int y = 0; y < csvMapData_.height; ++y) {
		for (int x = 0; x < csvMapData_.width; ++x) {
			MapChipType t = MapChipType::Empty;
			if (y < (int)csvMapData_.layer2.size() && x < (int)csvMapData_.layer2[y].size()) {
				t = csvMapData_.layer2[y][x];
			}
			if (!IsRenderable(t)) continue;

			// ←ここを「1段ぶんの高さ」に
			auto pos = tileToWorld(x, y, tileSize_);

			auto blk = std::make_unique<Block>();
			blk->Initialize(t, pos);
			// BlockMonkey ならテーブル登録
			if (t == MapChipType::BlockMonkey) {
				l2BlockAt_[y][x] = blk.get();
			}
			//blockScaleY_ = blk->GetScaleY();
			blocksL2_.push_back(std::move(blk));
		}
	}
}

std::optional<Vector3> Map::GetDogSpawnWorld() const
{
	// CsvLoader がセットした spawnDog を使う（見つからなければ x,y は負）
	const int gx = static_cast<int>(csvMapData_.spawnDog.x);
	const int gy = static_cast<int>(csvMapData_.spawnDog.y);
	if (gx >= 0 && gy >= 0) {
		return GridToWorld(gx, gy, 0.0f + blockScaleY_);     // 犬は1層高さ
	}
	return std::nullopt;
}

std::optional<Vector3> Map::GetMonkeySpawnWorld() const
{
	const int gx = static_cast<int>(csvMapData_.spawnMonkey.x);
	const int gy = static_cast<int>(csvMapData_.spawnMonkey.y);
	if (gx >= 0 && gy >= 0) {
		return GridToWorld(gx, gy, tileSize_ + blockScaleY_); // 猿は2層高さ
	}
	return std::nullopt;
}

std::optional<GridPos> Map::GetDogSpawnGrid() const
{
	if (csvMapData_.spawnDog.x >= 0 && csvMapData_.spawnDog.y >= 0)
		return GridPos{ (int)csvMapData_.spawnDog.x, (int)csvMapData_.spawnDog.y };
	return std::nullopt;
}

std::optional<GridPos> Map::GetMonkeySpawnGrid() const
{
	if (csvMapData_.spawnMonkey.x >= 0 && csvMapData_.spawnMonkey.y >= 0)
		return GridPos{ (int)csvMapData_.spawnMonkey.x, (int)csvMapData_.spawnMonkey.y };
	return std::nullopt;
}

bool Map::IsWalkableFor(ActorKind who, int gx, int gy) const
{
	// 範囲チェック
	if (gx < 0 || gy < 0 || gx >= csvMapData_.width || gy >= csvMapData_.height) return false;

	// 判定するレイヤ（犬=1層 / 猿=2層）
	const auto& layer = (who == ActorKind::Dog) ? csvMapData_.layer1 : csvMapData_.layer2;
	MapChipType t = layer[gy][gx];

	if (who == ActorKind::Dog) {
		// ---- 犬は layer1 を基準に判定
		const auto& L1 = csvMapData_.layer1;
		const auto& L2 = csvMapData_.layer2; // ← ★ 同じ(x,y)の layer2 も確認する
		MapChipType t1 = L1[gy][gx];

		// まずは「犬が歩けるタイル」か？
		bool baseOK =
			(t1 == MapChipType::StartDog) ||
			(t1 == MapChipType::StartMonkey) ||
			(t1 == MapChipType::FloorDog) ||
			(t1 == MapChipType::BlockMonkey) ||
			(t1 == MapChipType::GoalDog)/* ||
			(t1 == MapChipType::GoalMonkey)*/;

		if (!baseOK) return false;

		// 同じグリッドの layer2 に BlockMonkey がある場合は天井衝突扱いで NG
		MapChipType t2 = L2[gy][gx];
		if (t2 == MapChipType::FloorMonkey || t2 == MapChipType::GoalMonkey) {
			return false;
		}
		return true;
	}
	else { // Monkey（従来どおり layer2 基準）
		const auto& L2 = csvMapData_.layer2;
		MapChipType t2 = L2[gy][gx];
		switch (t2) {
		case MapChipType::StartDog:
		case MapChipType::StartMonkey:
		case MapChipType::FloorDog:
		case MapChipType::FloorMonkey:
		case MapChipType::BlockMonkey:
		case MapChipType::GoalDog:
		case MapChipType::GoalMonkey:
			return true;
		default:
			return false;
		}
	}
}

Vector3 Map::WorldFromGridFor(ActorKind who, int gx, int gy) const
{
	const float y = (who == ActorKind::Dog) ? (0.0f + blockScaleY_) : (tileSize_ + blockScaleY_);
	return GridToWorld(gx, gy, y);
}

bool Map::TryPushBlockByDog(int dogGx, int dogGy, int dx, int dy, const GridPos& monkeyPos)
{
	const int nx = dogGx + dx; // 犬の次マス（箱の現在地想定）
	const int ny = dogGy + dy;
	const int tx = nx + dx;    // 箱の移動先
	const int ty = ny + dy;

	// 範囲チェック
	if (nx < 0 || ny < 0 || nx >= csvMapData_.width || ny >= csvMapData_.height) return false;
	if (tx < 0 || ty < 0 || tx >= csvMapData_.width || ty >= csvMapData_.height) return false;

	// 次マスに箱があるか？（layer2）
	if (csvMapData_.layer2[ny][nx] != MapChipType::BlockMonkey) return false;

	// その箱の上にサルが乗っていたら押せない
	if (monkeyPos.x == nx && monkeyPos.y == ny) {
		return false;
	}

	// 押し先が空いているか？
	MapChipType dest = csvMapData_.layer2[ty][tx];

	// FloorMonkey / GoalMonkey は押し先として禁止
	if (dest == MapChipType::FloorMonkey || dest == MapChipType::GoalMonkey) {
		return false;
	}

	// 許可は Empty のみ（他は全部NG：Start*, BlockMonkey なども不可）
	if (dest != MapChipType::Empty) {
		return false;
	}

	// ここから先は従来どおり（グリッド更新・テーブル更新・見た目移動）
	Block* block = l2BlockAt_[ny][nx];
	if (!block) return false;

	csvMapData_.layer2[ny][nx] = MapChipType::Empty;
	csvMapData_.layer2[ty][tx] = MapChipType::BlockMonkey;

	l2BlockAt_[ny][nx] = nullptr;
	l2BlockAt_[ty][tx] = block;

	Vector3 newPos = GridToWorld(tx, ty, tileSize_);
	block->SetWorldPosition(newPos);

	return true;

}

bool Map::HasBlockMonkeyAt(int gx, int gy) const
{
	if (gx < 0 || gy < 0 || gx >= csvMapData_.width || gy >= csvMapData_.height) return false;
	return csvMapData_.layer2[gy][gx] == MapChipType::BlockMonkey;
}

bool Map::IsRenderable(MapChipType t) {
	switch (t) {
	case MapChipType::FloorDog:
	case MapChipType::FloorMonkey:
	case MapChipType::BlockMonkey:
	case MapChipType::GoalDog:
	case MapChipType::GoalMonkey:
		return true;            // モデルがあるタイルだけ描画
	default:
		return false;
	}
}

Vector3 Map::GridToWorld(int gx, int gy, float yOffset) const
{
	// CSVは上→下の行順なので、既存描画と同じくY軸を反転
	int flippedY = (csvMapData_.height - 1) - gy;
	return Vector3{
		origin_.x + gx * tileSize_,
		origin_.y + yOffset,
		origin_.z + flippedY * tileSize_
	};
}

std::optional<Vector3> Map::FindStartOnLayer(const std::vector<std::vector<MapChipType>>& layer, MapChipType target, float yOffset) const
{
	for (int y = 0; y < (int)layer.size(); ++y) {
		for (int x = 0; x < (int)layer[y].size(); ++x) {
			if (layer[y][x] == target) {
				return GridToWorld(x, y, yOffset);
			}
		}
	}
	return std::nullopt;
}
