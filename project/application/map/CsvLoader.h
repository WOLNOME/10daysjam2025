#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "MapChipType.h"
#include "Vector2.h"


struct CsvMapData {
	// 一層（犬のフロア）のマップデータ
	std::vector<std::vector<MapChipType>> layer1;
	// 二層（猿のフロア）のマップデータ
	std::vector<std::vector<MapChipType>> layer2;
	// マップの大きさ
	int width = 0;
	int height = 0;
	// スポーン場所 (マップチップ座標) (Redo,Undoでプレイヤーの座標として流用)
	Vector2 spawnDog = { -1,-1 };
	Vector2 spawnMonkey = { -1,-1 };
	// ゴール場所 (マップチップ座標)
	Vector2 goalDog{ -1, -1 };
	Vector2 goalMonkey{ -1, -1 };
	// -1は未設定の場合画面外に行くように
};
class CsvLoader
{
	

public:
	
	// 初期化
	void Initialize();
	// CSVファイル読み込み
	CsvMapData Loader(const std::string& filepath);

private:
	// マップの基本情報
	CsvMapData csvMapData_;


};

