#include "TitleScene.h"

void TitleScene::Initialize() {
	//シーン共通の初期化
	BaseScene::Initialize();
	input_ = Input::GetInstance();
	//カメラの生成と初期化
	camera_ = std::make_unique<GameCamera>();
	camera_->Initialize();
	camera_->SetFarClip(500.0f);
	//カメラのセット
	Object3dManager::GetInstance()->SetCamera(camera_.get());
	LineManager::GetInstance()->SetCamera(camera_.get());
	ParticleManager::GetInstance()->SetCamera(camera_.get());
	//シーンライトのセット
	Object3dManager::GetInstance()->SetSceneLight(sceneLight_.get());

	//タイトルシステムの生成と初期化
	titleSystem_ = std::make_unique<TitleSystem>();
	titleSystem_->Initialize();
}

void TitleScene::Finalize() {
}

void TitleScene::Update() {
	//シーン共通の更新
	BaseScene::Update();

	//タイトルシステムの更新
	titleSystem_->Update();


	//カメラ更新
	camera_->Update();
}

void TitleScene::DebugWithImGui() {
#ifdef _DEBUG
	titleSystem_->DebugWithImGui();

#endif // _DEBUG
}
