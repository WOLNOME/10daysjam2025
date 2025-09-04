#include "SceneFactory.h"
#include "DevelopScene.h"
#include "ParticleEditorScene.h"

//アプリケーション
#include <application/scene/TitleScene.h>
#include <application/scene/MenuScene.h>
#include <application/scene/StageSelectScene.h>
#include <application/scene/GamePlayScene.h>
#include <application/scene/TutorialScene.h>

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "DEVELOP") {
		newScene = new DevelopScene();
	}
	else if (sceneName == "TITLE") {
		newScene = new TitleScene();
	}
	else if (sceneName == "MENU") {
		newScene = new MenuScene();
	}
	else if (sceneName == "STAGESELECT") {
		newScene = new StageSelectScene();
	}
	else if(sceneName == "GAMEPLAY") {
		newScene = new GamePlayScene();
	}
	else if(sceneName == "TUTORIAL") {
		newScene = new TutorialScene();
	}
	//パーティクルエディター
	else if (sceneName == "PARTICLEEDITOR") {
		newScene = new ParticleEditorScene();
	}

	return newScene;
}
