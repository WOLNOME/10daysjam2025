#pragma once
#include "Block.h"
#include "CsvLoader.h"
#include "Vector3.h"
class Map
{

public:
	// 初期化
	void Initialize(const std::string& filepath);


private:

	// 一層
	std::vector<std::unique_ptr<Block>> blocksL1_;
	// 二層
	std::vector<std::unique_ptr<Block>> blocksL2_;

	// タイル→ワールド変換
	float   tileSize_ = 2.0f;
	Vector3 origin_ = { 0,0,0 };
	CsvMapData csvMapData_;
	float   overlayYOffset_ = 0.01f;
};

