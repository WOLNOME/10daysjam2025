#include "SceneManager.h"
#include "SceneFactory.h"
#include "ImGuiManager.h"
#include <cassert>

SceneManager* SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance() {
	if (instance == nullptr) {
		instance = new SceneManager;
	}
	return instance;
}

void SceneManager::Initialize() {
	//シーンファクトリーの生成
	sceneFactory_ = new SceneFactory();
	//シーン遷移アニメーションの生成
	sceneTransitionAnimation_ = std::make_unique<SceneTransitionAnimation>();
	sceneTransitionAnimation_->Initialize();

	//BGM生成
	menuBGM_ = std::make_unique<Audio>();
	menuBGM_->Initialize("menuBGM.wav");
	gamePlayBGM_ = std::make_unique<Audio>();
	gamePlayBGM_->Initialize("gamePlayBGM.wav");

}

void SceneManager::Update() {
	//シーン遷移アニメーションの更新
	sceneTransitionAnimation_->Update();
	//シーン切り替え処理
	ChangeScene();
	//実行中シーンを更新する
	scene_->Update();
}

void SceneManager::DebugWithImGui() {
#ifdef _DEBUG
	//FPS表示
	scene_->ShowFPS();
	//デバッグ処理
	scene_->DebugWithImGui();
#endif // _DEBUG

}

void SceneManager::CurtainDraw() {
	//シーンの遷移幕描画
	sceneTransitionAnimation_->Draw();
}

void SceneManager::Finalize() {
	//最後のシーンの終了と解放
	scene_->Finalize();
	delete scene_;
	//シーンファクトリー解放
	delete sceneFactory_;
	//インスタンスの削除
	delete instance;
	instance = nullptr;
}

void SceneManager::ChangeScene() {
	//次のシーン予約があるなら
	if (nextScene_ && !sceneTransitionAnimation_->IsTransitioning()) {
		//遷移アニメーション開始
		sceneTransitionAnimation_->StartTransition();
	}
	//遷移アニメーション中なら
	if (sceneTransitionAnimation_->GetState() == SceneTransitionAnimation::TransitionState::UPDATE_IN) {
		//フェードイン処理
		sceneTransitionAnimation_->UpdateIn();
	}
	else if (sceneTransitionAnimation_->GetState() == SceneTransitionAnimation::TransitionState::END_IN) {
		//フェードイン終了
		sceneTransitionAnimation_->EndIn();
		//旧シーンの終了
		if (scene_) {
			scene_->Finalize();
			delete scene_;
		}
		//シーンの切り替え
		scene_ = nextScene_;
		nextScene_ = nullptr;
		//次のシーンを初期化する
		scene_->Initialize();
	}
	else if (sceneTransitionAnimation_->GetState() == SceneTransitionAnimation::TransitionState::UPDATE_OUT) {
		//フェードアウト処理
		sceneTransitionAnimation_->UpdateOut();
	}
	else if (sceneTransitionAnimation_->GetState() == SceneTransitionAnimation::TransitionState::END_OUT) {
		//フェードアウト終了
		sceneTransitionAnimation_->EndOut();
	}
	else if (sceneTransitionAnimation_->GetState() == SceneTransitionAnimation::TransitionState::END_ALL) {
		//フェードアウト終了
		sceneTransitionAnimation_->EndAll();
	}
}

bool SceneManager::SetNextScene(const std::string& nextSceneName, SceneTransitionAnimation::TransitionType transitionType, uint32_t frame) {
	//遷移中なら何もしない
	if (sceneTransitionAnimation_->IsTransitioning()) return false;

	//警告
	assert(sceneFactory_);
	assert(nextScene_ == nullptr);

	//BGM再生
	if (nextSceneName == "MENU" || nextSceneName == "STAGESELECT" || nextSceneName == "TITLE") {
		//最初のシーン生成時
		if (!menuBGM_->IsPlaying() && !gamePlayBGM_->IsPlaying()) {
			menuBGM_->Play(true, 0.5f);
		}
		//ゲームシーン→メニューシーン時の処理
		else if (!menuBGM_->IsPlaying() && gamePlayBGM_->IsPlaying()) {
			gamePlayBGM_->Stop();
			menuBGM_->Play(true, 0.5f);
		}
	}
	else {
		//メニューシーン→ゲームシーン時の処理
		if (!gamePlayBGM_->IsPlaying() && menuBGM_->IsPlaying()) {
			menuBGM_->Stop();
			gamePlayBGM_->Play(true, 0.5f);
		}
	}

	//もし最初のシーンだったらここで生成＆初期化
	if (scene_ == nullptr) {
		scene_ = sceneFactory_->CreateScene(nextSceneName);
		nextScene_ = nullptr;
		scene_->Initialize();
		return true;
	}

	//次シーンを生成
	nextScene_ = sceneFactory_->CreateScene(nextSceneName);
	//遷移アニメーションタイプを設定
	sceneTransitionAnimation_->SetTransitionType(transitionType);
	//遷移アニメーションフレームを設定
	sceneTransitionAnimation_->SetFrame(frame);

	return true;
}
