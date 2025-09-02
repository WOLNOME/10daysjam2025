#include "DevelopScene.h"
#include "SceneManager.h"
#include <numbers>

void DevelopScene::Initialize() {
	//シーン共通の初期化
	BaseScene::Initialize();

	input_ = Input::GetInstance();

	//カメラの生成と初期化
	camera = std::make_unique<DevelopCamera>();
	camera->Initialize();
	camera->worldTransform.rotate = (cameraRotate);
	camera->worldTransform.translate = (cameraTranslate);
	camera->SetFarClip(500.0f);

	//平行光源の生成と初期化
	dirLight = std::make_unique<DirectionalLight>();
	//点光源の生成と初期化
	pointLight = std::make_unique<PointLight>();
	pointLight2 = std::make_unique<PointLight>();
	//スポットライトの生成と初期化
	spotLight = std::make_unique<SpotLight>();

	//各光源をシーンライトにセット
	sceneLight_->SetLight(dirLight.get());
	sceneLight_->SetLight(pointLight.get());
	sceneLight_->SetLight(pointLight2.get());
	sceneLight_->SetLight(spotLight.get());

	//シーン変数の初期化
	sprite_ = std::make_unique<Sprite>();
	textureHandleSprite_ = TextureManager::GetInstance()->LoadTexture("monsterBall.png");
	sprite_->Initialize(SpriteManager::GetInstance()->GenerateName("MonsterBall"), Sprite::Order::Front0, textureHandleSprite_);
	sprite_->SetAnchorPoint({ 0.5f,0.5f });
	sprite_->SetFlipX(true);
	sprite_->SetIsDisplay(false);

	sprite2_ = std::make_unique<Sprite>();
	textureHandleSprite2_ = TextureManager::GetInstance()->LoadTexture("monsterBall.png");
	sprite2_->Initialize(SpriteManager::GetInstance()->GenerateName("MonsterBall"), Sprite::Order::Front0, textureHandleSprite2_);
	sprite2Position = { 100.0f,100.0f };
	sprite2_->SetPosition(sprite2Position);
	sprite2_->SetSize({ 300.0f,300.0f });
	sprite2_->SetIsDisplay(false);

	//スカイボックスの生成と初期化
	textureHandleSkyBox_ = TextureManager::GetInstance()->LoadTexture("skybox.dds");
	skyBox_ = std::make_unique<Object3d>();
	skyBox_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("SkyBox"), Shape::ShapeKind::kSkyBox);
	skyBox_->worldTransform.scale = { 300.0f,300.0f,300.0f };
	skyBox_->SetTexture(textureHandleSkyBox_);

	//3Dオブジェクトの生成と初期化
	teapot_ = std::make_unique<Object3d>();
	teapot_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Teapot"), "teapot");
	int32_t elthTeapot = TextureManager::GetInstance()->LoadTexture("skybox.dds");
	teapot_->SetEnvironmentLightTextureHandle(elthTeapot);

	terrain_ = std::make_unique<Object3d>();
	terrain_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Terrain"), "terrain");
	terrain_->worldTransform.translate = { 0.0f,-1.2f,0.0f };

	animatedCube_ = std::make_unique<Object3d>();
	animatedCube_->Initialize(AnimationModelTag{}, Object3dManager::GetInstance()->GenerateName("AnimatedCube"), "AnimatedCube");
	animatedCube_->worldTransform.translate = { 0.0f,3.0f,0.0f };

	sneakWalk_ = std::make_unique<Object3d>();
	sneakWalk_->Initialize(AnimationModelTag{}, Object3dManager::GetInstance()->GenerateName("SneakWalk"), "sneakWalk");
	sneakWalk_->worldTransform.translate = { 3.0f,3.0f,0.0f };
	sneakWalk_->SetNewAnimation("sneakWalk", "sneakWalk");
	sneakWalk_->SetCurrentAnimation("sneakWalk");

	composite_ = std::make_unique<Object3d>();
	composite_->Initialize(AnimationModelTag{}, Object3dManager::GetInstance()->GenerateName("Composite"), "walk");
	composite_->worldTransform.translate = { 0.0f,1.0f,-3.0f };
	composite_->worldTransform.rotate = { 0.0f,3.14f,0.0f };
	composite_->SetNewAnimation("walk", "walk");
	composite_->SetNewAnimation("sneakWalk", "sneakWalk");
	composite_->SetCurrentAnimation("walk");

	walk_ = std::make_unique<Object3d>();
	walk_->Initialize(AnimationModelTag{}, Object3dManager::GetInstance()->GenerateName("Walk"), "walk");
	int32_t elthWalk = TextureManager::GetInstance()->LoadTexture("skybox.dds");
	walk_->SetEnvironmentLightTextureHandle(elthWalk);
	walk_->worldTransform.translate = { 4.0f,3.0f,0.0f };
	walk_->SetNewAnimation("walk", "walk");
	walk_->SetCurrentAnimation("walk");

	simpleSkin_ = std::make_unique<Object3d>();
	simpleSkin_->Initialize(AnimationModelTag{}, Object3dManager::GetInstance()->GenerateName("SimpleSkin"), "simpleSkin");
	simpleSkin_->worldTransform.translate = { 5.0f,3.0f,0.0f };

	//レベルオブジェクトの生成・初期化
	//levelObject_ = std::make_unique<LevelLoader>();
	//levelObject_->Initialize();

	//パーティクルの生成・初期化
	particle_ = std::make_unique<Particle>();
	particle_->Initialize(ParticleManager::GetInstance()->GenerateName("Particle"), "gpu");

	//オーディオの生成・初期化
	audio_ = std::make_unique<Audio>();
	audio_->Initialize("demo2.wav");

	//カメラのセット
	Object3dManager::GetInstance()->SetCamera(camera.get());
	LineManager::GetInstance()->SetCamera(camera.get());
	ParticleManager::GetInstance()->SetCamera(camera.get());
	//シーンライトのセット
	Object3dManager::GetInstance()->SetSceneLight(sceneLight_.get());

	//テキストテクスチャの作成
	{
		TextParam param;
		param.text = L"フォント確認 0123 abcDEF";
		param.font = Font::UDDegitalNP_B;
		param.fontStyle = FontStyle::Normal;
		param.size = 32.0f;
		param.color = { 1,1,1,1 };
		textHandle_ = TextTextureManager::GetInstance()->LoadTextTexture(param);
		text_ = std::make_unique<Sprite>();
		text_->Initialize(SpriteManager::GetInstance()->GenerateName("Text"), Sprite::Order::Front0);
		text_->SetTexture(textHandle_);
		text_->SetPosition({ 640,360 });
		text_->SetAnchorPoint({ 0.5f,0.5f });
		EdgeParam edgeParam;
		edgeParam.width = 2;
		edgeParam.isEdgeDisplay = 1;
		edgeParam.slideRate = { 0.0f,0.0f };
		edgeParam.color = { 1,1,0,1 };
		TextTextureManager::GetInstance()->EditEdgeParam(textHandle_, edgeParam);
	}
}

void DevelopScene::Finalize() {
}

void DevelopScene::Update() {
	//シーン共通の更新
	BaseScene::Update();

	//TABでシーン再読み込み
	if (input_->TriggerKey(DIK_TAB)) {
		sceneManager_->SetNextScene("DEVELOP");
	}

	//カメラの更新
	camera->Update();

	//ティーポットの回転
	teapot_->worldTransform.rotate.y += 0.03f;
	//レベルオブジェクトの更新
	//levelObject_->Update();

	//スプライトの更新
	sprite_->SetRotation(sprite_->GetRotation() + 0.03f);

	time_ += kDeltaTime;
	TextTextureManager::GetInstance()->EditTextString(textHandle_, L"フォント確認 0123 abcDEF\n現在時刻 : {:.1f}", time_);

}

void DevelopScene::DebugWithImGui() {
#ifdef _DEBUG
	//オーディオのデバッグ用ImGui
	audio_->DebugWithImGui(L"アラーム");

	//平行光源のデバッグ用ImGui
	dirLight->DebugWithImGui(L"1");

	//点光源のデバッグ用ImGui
	pointLight->DebugWithImGui(L"1");
	pointLight2->DebugWithImGui(L"2");

	//スポットライトのデバッグ用ImGui
	spotLight->DebugWithImGui(L"1");

	ImGui::Begin("複合アニメーション");
	//選択肢
	const char* items[] = { "walk","sneakWalk" };
	static int currentItem = 0;
	if (ImGui::Combo("アニメーションのアイテム", &currentItem, items, IM_ARRAYSIZE(items))) {
		composite_->SetCurrentAnimation(items[currentItem]);
	}
	ImGui::End();

	//レベルオブジェクト用ImGui
	//levelObject_->DebugWithImGui();
	//テキスト用ImGui
	TextTextureManager::GetInstance()->DebugWithImGui(textHandle_);
	//カメラ用ImGui
	camera->DebugWithImGui();
	//ポストエフェクト用ImGui
	PostEffectManager::GetInstance()->DebugWithImGui();

#endif // _DEBUG
}
