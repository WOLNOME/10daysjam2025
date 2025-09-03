#include "SceneFactory.h"
#include "DevelopScene.h"
#include "ParticleEditorScene.h"

//アプリケーション
#include <application/scene/GamePlayScene.h>


BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "DEVELOP") {
		newScene = new DevelopScene();
	}
	else if(sceneName == "GAMEPLAY") {
		newScene = new GamePlayScene();
	}
	//パーティクルエディター
	else if (sceneName == "PARTICLEEDITOR") {
		newScene = new ParticleEditorScene();
	}

	return newScene;
}
