#include "SceneFactory.h"
#include "DevelopScene.h"
#include "ParticleEditorScene.h"


BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "DEVELOP") {
		newScene = new DevelopScene();
	}
	//パーティクルエディター
	else if (sceneName == "PARTICLEEDITOR") {
		newScene = new ParticleEditorScene();
	}

	return newScene;
}
