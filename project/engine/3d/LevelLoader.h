#pragma once
#include <LevelObject.h>
#include <JsonUtil.h>
#include <list>
#include <string>
#include <memory>

class LevelLoader {
private://構造体
	//自キャラの生成データ
	struct PlayerSpawnData {
		Vector3 translation; //位置
		Vector3 rotation; //回転角
	};
	//敵キャラの生成データ
	struct EnemySpawnData {
		std::string fileName;	//ファイル名(敵の区別をするため)
		Vector3 translation;	//平行移動
		Vector3 rotation;		//回転角
	};

	//レベルデータ
	struct LevelData {
		//オブジェクトのコンテナ
		std::list<std::unique_ptr<LevelObject>> objects;
		//自キャラコンテナ
		std::list<PlayerSpawnData> players;
		//敵キャラコンテナ
		std::list<EnemySpawnData> enemies;
	};

public:
	//初期化(.jsonは省略)
	void Initialize();
	//更新
	void Update();
	//デバッグ
	void DebugWithImGui();

public://getter
	//プレイヤースポーンデータの取得
	const std::list<PlayerSpawnData>& GetPlayerSpawnData() const { return levelData_.players; }
	//敵スポーンデータの取得
	const std::list<EnemySpawnData>& GetEnemySpawnData() const { return levelData_.enemies; }

private://非公開メンバ関数
	//オブジェクトデータのロード
	void ScanObjectData(json& object);

private:
	//レベルデータ
	LevelData levelData_;

};

