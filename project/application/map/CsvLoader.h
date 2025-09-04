#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "MapChipType.h"
#include "Vector2.h"
struct CsvMapData {
	// 2D配列でマップデータを保持
	std::vector<std::vector<MapChipType>> layer1;
	std::vector<std::vector<MapChipType>> layer2;
	// マップの大きさ
	int width = 0;
	int height = 0;
	// スポーン場所 (マップチップ座標)
	Vector2 spawnDog = { -1,-1 };
	Vector2 spawnMonkey = { -1,-1 };
	// ゴール場所 (マップチップ座標)
	Vector2 goalDog{ -1, -1 };
	Vector2 goalMonkey{ -1, -1 };
};
class CsvLoader
{
	

public:
	
	// 初期化
	void Initialize();

	CsvMapData Loader(const std::string& filepath);

private:

	CsvMapData csvMapData_;


};

