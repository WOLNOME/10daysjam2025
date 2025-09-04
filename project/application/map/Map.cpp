#include "Map.h"

namespace {
	inline bool IsRenderable(MapChipType t) {
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
}

void Map::Initialize(const std::string& filepath) {
	CsvLoader loader;
	loader.Initialize();
	csvMapData_ = loader.Loader(filepath); 

	blocksL1_.clear();
	blocksL2_.clear();



	
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
			blockScaleY_ = blk->GetScaleY();
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
