#include "GamePlayScene.h"

std::string GamePlayScene::pendingStageName_{};

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

	//map_ = std::make_unique<Map>();
	////map_->Initialize("Dog_Tutorial");
	//map_->Initialize("Stage_20");

	// Initialize() 内の map 初期化部分を置き換え
	map_ = std::make_unique<Map>();

	// 受け皿に値が来ていればそれを使う。空ならデフォルトにフォールバック
	std::string mapName = pendingStageName_.empty() ? "Stage_1" : pendingStageName_;
	map_->Initialize(mapName.c_str());

	// 使い終わったらクリアしておく（任意）
	pendingStageName_.clear();

	StageCamera stageCamera = map_->GetStageCameraVal();
	camera_->worldTransform.translate = stageCamera.overLooking.position;
	camera_->worldTransform.rotate = stageCamera.overLooking.rotate;

	player_ = std::make_unique<Player>();
	player_->Initialize(*map_);
	//ポーズシステムの生成と初期化
	poseSystem_ = std::make_unique<PoseSystem>();
	poseSystem_->Initialize();
	
	//背景平面の生成と初期化
	backPlane_ = std::make_unique<BackPlane>();
	backPlane_->Initialize();
	
}

void GamePlayScene::Finalize() {

}

void GamePlayScene::Update() {
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

	player_->Update(*map_);

	backPlane_->Update();
}

void GamePlayScene::DebugWithImGui() {
#ifdef _DEBUG
	poseSystem_->DebugWithImGui();
	ImGui::Separator();
	camera_->DebugWithImGui();

#endif // _DEBUG
}
