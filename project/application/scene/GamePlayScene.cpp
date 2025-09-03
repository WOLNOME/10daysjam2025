#include "GamePlayScene.h"

void GamePlayScene::Initialize() {
	//シーン共通の初期化
	BaseScene::Initialize();
	input_ = Input::GetInstance();
	//カメラの生成と初期化
	camera_ = std::make_unique<DevelopCamera>();
	camera_->Initialize();
	camera_->SetFarClip(500.0f);
	//カメラのセット
	Object3dManager::GetInstance()->SetCamera(camera_.get());
	LineManager::GetInstance()->SetCamera(camera_.get());
	ParticleManager::GetInstance()->SetCamera(camera_.get());
	//シーンライトのセット
	Object3dManager::GetInstance()->SetSceneLight(sceneLight_.get());

	monkey_ = std::make_unique<Monkey>();
	monkey_->Initialize();
}

void GamePlayScene::Finalize() {
}

void GamePlayScene::Update() {
	//シーン共通の更新
	BaseScene::Update();
	//カメラ更新
	camera_->Update();

	monkey_->Update();
}

void GamePlayScene::DebugWithImGui() {
#ifdef _DEBUG

#endif // _DEBUG
}
