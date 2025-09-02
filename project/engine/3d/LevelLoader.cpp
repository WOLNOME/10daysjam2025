#include "LevelLoader.h"
#include <assert.h>

void LevelLoader::Initialize() {
	//jsonデータを読み込む
	json data = JsonUtil::GetJsonData("Resources/levelData/levelData");
	//全オブジェクトデータのロード
	for (json& object : data["objects"]) {
		ScanObjectData(object);
	}
}

void LevelLoader::Update() {
	//オブジェクトの更新
	for (const auto& object : levelData_.objects) {
		object->Update();
	}
}

void LevelLoader::DebugWithImGui() {
#ifdef _DEBUG
	//全オブジェクトのデバッグ処理
	for (const auto& object : levelData_.objects) {
		object->DebugWithImGui();
	}
#endif // _DEBUG

}

void LevelLoader::ScanObjectData(json& object) {
	//ラムダ式でオブジェクト生成
	auto createObject = [](json& object, LevelData& levelData, const std::string& name, const std::string& fileName) {
		std::unique_ptr<LevelObject> addObject;
		addObject = std::make_unique<LevelObject>();
		//トランスフォームパラメータの読み込み
		TransformEuler transform;
		//平行移動
		transform.translate.x = (float)object["transform"]["translation"][0];
		transform.translate.y = (float)object["transform"]["translation"][2];
		transform.translate.z = (float)object["transform"]["translation"][1];
		//回転角
		transform.rotate.x = -(float)object["transform"]["rotation"][0];
		transform.rotate.y = -(float)object["transform"]["rotation"][2];
		transform.rotate.z = -(float)object["transform"]["rotation"][1];
		//スケーリング
		transform.scale.x = (float)object["transform"]["scaling"][0];
		transform.scale.y = (float)object["transform"]["scaling"][2];
		transform.scale.z = (float)object["transform"]["scaling"][1];
		addObject->Initialize(name, fileName, transform);

		//コライダー
		if (object.contains("collider")) {
			//コリジョン情報の読み込み
			json& collision = object["collider"];
			Vector3 center = { (float)collision["center"][0], (float)collision["center"][2], (float)collision["center"][1] };
			Vector3 size = { (float)collision["size"][0], (float)collision["size"][2], (float)collision["size"][1] };
			addObject->SetCollisionInfo(center, size);
		}

		//オブジェクトリストに登録
		levelData.objects.push_back(std::move(addObject));
		};

	//"type"データがない場合不正データのため警告
	assert(object.contains("type"));

	//種別を取得
	std::string type = object["type"].get<std::string>();

	//メッシュオブジェクトの処理
	if (type.compare("MESH") == 0) {
		createObject(object, levelData_, "mesh", object["file_name"].get<std::string>());
	}
	//自キャラ発生ポイント
	else if (type.compare("PlayerSpawn") == 0) {
		PlayerSpawnData data;

		//トランスフォームパラメータの読み込み
		json& transform = object["transform"];
		//平行移動
		data.translation.x = (float)transform["translation"][0];
		data.translation.y = (float)transform["translation"][2];
		data.translation.z = (float)transform["translation"][1];
		//回転角
		data.rotation.x = -(float)transform["rotation"][0];
		data.rotation.y = -(float)transform["rotation"][2];
		data.rotation.z = -(float)transform["rotation"][1];

		//自キャラコンテナに登録
		levelData_.players.push_back(data);
	}
	//敵キャラ発生ポイント
	else if (type.compare("EnemySpawn") == 0) {
		EnemySpawnData data;
		//トランスフォームパラメータの読み込み
		json& transform = object["transform"];
		//平行移動
		data.translation.x = (float)transform["translation"][0];
		data.translation.y = (float)transform["translation"][2];
		data.translation.z = (float)transform["translation"][1];
		//回転角
		data.rotation.x = -(float)transform["rotation"][0];
		data.rotation.y = -(float)transform["rotation"][2];
		data.rotation.z = -(float)transform["rotation"][1];
		//ファイル名を取得
		data.fileName = object["file_name"].get<std::string>();
		//敵キャラコンテナに登録
		levelData_.enemies.push_back(data);
	}
	//ツリー生成ポイント
	else if (type.compare("TreeObject") == 0) {
		createObject(object, levelData_, "TreeObject", "tree");
	}

	//子オブジェクトがある場合はその処理も行う(ペアレント処理は割愛→必要に応じて付け加える)
	if (object.contains("children")) {
		for (json& childObject : object["children"]) {
			ScanObjectData(childObject);
		}
	}
}
