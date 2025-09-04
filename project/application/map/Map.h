#pragma once
#include "Block.h"
#include "CsvLoader.h"
#include "Vector3.h"
class Map
{

public:
	// 初期化
	void Initialize(const std::string& filepath);

	// 犬/猿のスポーン位置（ワールド座標）を返す
	std::optional<Vector3> GetDogSpawnWorld() const;
	std::optional<Vector3> GetMonkeySpawnWorld() const;


private:

	// 一層
	std::vector<std::unique_ptr<Block>> blocksL1_;
	// 二層
	std::vector<std::unique_ptr<Block>> blocksL2_;
;
	// タイル→ワールド変換
	float   tileSize_ = 2.0f;
	Vector3 origin_ = { 0,0,0 };
	CsvMapData csvMapData_;
	float   overlayYOffset_ = 0.01f;
	// 床ブロックの高さ（scale.y）
	float   blockScaleY_ = 1.0f;

	// グリッド→ワールド座標変換
	Vector3 GridToWorld(int gx, int gy, float yOffset) const;
	std::optional<Vector3> FindStartOnLayer(
		const std::vector<std::vector<MapChipType>>& layer,
		MapChipType target, float yOffset) const;
};

