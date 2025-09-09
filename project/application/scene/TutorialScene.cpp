#include "TutorialScene.h"

void TutorialScene::Initialize() {
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

	map_ = std::make_unique<Map>();
	map_->Initialize("Monkey_Tutorial");
	player_ = std::make_unique<Player>();
	player_->Initialize(*map_);

	//ポーズシステムの生成と初期化
	poseSystem_ = std::make_unique<PoseSystem>();
	poseSystem_->Initialize();
}

void TutorialScene::Finalize() {
}

void TutorialScene::Update() {
	//シーン共通の更新
	BaseScene::Update();

	//ポーズシステム更新
	poseSystem_->Update();
	//もしもポーズ中なら
	if (poseSystem_->GetIsPosing()) {
		return;
	}

	///========================	///
	/// ↓↓処理はこの下に書く↓↓	///
	///========================	///		



	//カメラ更新
	camera_->Update();

	//プレイヤー更新
	player_->Update(*map_);
}

void TutorialScene::DebugWithImGui() {
#ifdef _DEBUG
	poseSystem_->DebugWithImGui();
#endif // _DEBUG
}
