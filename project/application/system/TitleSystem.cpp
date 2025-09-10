#include "TitleSystem.h"
#include <SceneManager.h>
#include <SpriteManager.h>
#include <TextureManager.h>
#include <TextTextureManager.h>
#include <imgui.h>

void TitleSystem::Initialize() {
	//インプット
	input_ = Input::GetInstance();

	//背景スプライト生成
	/*backSprite_ = std::make_unique<Sprite>();
	backTexture_ = TextureManager::GetInstance()->LoadTexture("black.png");
	backSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("back"), Sprite::Order::Back0, backTexture_);
	backSprite_->SetPosition({ 0,0 });
	backSprite_->SetAnchorPoint({ 0,0 });*/
	//タイトル文字スプライト生成
	titleTextSprite_ = std::make_unique<Sprite>();
	TextParam titleTextParam;
	titleTextParam.text = L"すれ違い協定";
	titleTextParam.font = Font::UDDegitalN_B;
	titleTextParam.fontStyle = FontStyle::Normal;
	titleTextParam.size = 96.0f;
	titleTextParam.color = { 1.0f,1.0f,1.0f,1.0f };
	titleTextTexture_ = TextTextureManager::GetInstance()->LoadTextTexture(titleTextParam);
	titleTextSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("titleText"), Sprite::Order::Back1);
	titleTextSprite_->SetTexture(titleTextTexture_);
	titleTextSprite_->SetPosition({ 640,180 });
	titleTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
	//次へ進むUIスプライト生成
	nextUITextSprite_ = std::make_unique<Sprite>();
	TextParam nextUITextParam;
	nextUITextParam.text = L"[SPACE]または(A)を押してください";
	nextUITextParam.font = Font::UDDegitalN_R;
	nextUITextParam.fontStyle = FontStyle::Normal;
	nextUITextParam.size = 32.0f;
	nextUITextParam.color = { 1.0f,1.0f,1.0f,1.0f };
	nextUITextTexture_ = TextTextureManager::GetInstance()->LoadTextTexture(nextUITextParam);
	nextUITextSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("nextUI"), Sprite::Order::Back1);
	nextUITextSprite_->SetTexture(nextUITextTexture_);
	nextUITextSprite_->SetPosition({ 640,600 });
	nextUITextSprite_->SetAnchorPoint({ 0.5f,0.5f });

	//決定音生成
	decideSE_ = std::make_unique<Audio>();
	decideSE_->Initialize("decide.wav");

	// 背景平面生成
	backPlane_ = std::make_unique<BackPlane>();
	backPlane_->Initialize();

	//犬生成
	dog_ = std::make_unique<Object3d>();
	dog_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Dog"), "dog");
	dog_->worldTransform.scale = { 0.1f,0.1f,0.1f };
	dog_->worldTransform.translate = { 0.5f,-0.2f,1.82f };
	dog_->worldTransform.rotate = { 0.0f,0.4f,0.0f };

	// 猿生成
	monkey_ = std::make_unique<Object3d>();
	monkey_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Monkey"), "monkey");
	monkey_->worldTransform.scale = { 0.1f,0.1f,0.1f };
	monkey_->worldTransform.translate = { -0.77f,-0.3f,2.75f };
	monkey_->worldTransform.rotate = { 0.0f,-0.4f,0.0f };

	// 犬ブロック生成
	dogBlock_ = std::make_unique<Object3d>();
	dogBlock_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("DogBlock"), "floorDog");
	dogBlock_->worldTransform.scale = { 0.1f,0.1f,0.1f };
	dogBlock_->worldTransform.translate = { 0.750f,-0.4f,2.72f };
	dogBlock_->worldTransform.rotate = { 0.0f,0.4f,0.0f };
	

	// 猿ブロック生成
	monkeyBlock_ = std::make_unique<Object3d>();
	monkeyBlock_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("MonkeyBlock"), "floorMonkey");
	monkeyBlock_->worldTransform.scale = { 0.1f,0.1f,0.1f };
	monkeyBlock_->worldTransform.translate = { -0.75f,-0.38f,2.72f };
	monkeyBlock_->worldTransform.rotate = { 0.0f,-0.6f,0.0f };

	dogBaseY_ = dog_->worldTransform.translate.y;
}

void TitleSystem::Update() {
	//スペースキーorBボタンで次のシーンへ
	if ((input_->TriggerKey(DIK_SPACE) || input_->TriggerPadButton(GamepadButton::ButtonA)) && !isSceneChanging_) {
		if (SceneManager::GetInstance()->SetNextScene("MENU")) {
			isSceneChanging_ = true;
			//決定音再生
			decideSE_->Play();
		}
	}
	
	backPlane_->Update();


	dog_->worldTransform.UpdateMatrix();
	monkey_->worldTransform.UpdateMatrix();
	dogBlock_->worldTransform.UpdateMatrix();
	monkeyBlock_->worldTransform.UpdateMatrix();

	// 犬猿ブロックのimgui
#ifdef _DEBUG
	ImGui::Begin("DogMonkey");
	ImGui::DragFloat3("Dog Translate", &dog_->worldTransform.translate.x, 0.01f);
	ImGui::DragFloat3("Dog Rotate", &dog_->worldTransform.rotate.x, 0.01f);
	ImGui::DragFloat3("Dog Scale", &dog_->worldTransform.scale.x, 0.01f);

	ImGui::DragFloat3("Monkey Translate", &monkey_->worldTransform.translate.x, 0.01f);
	ImGui::DragFloat3("Monkey Rotate", &monkey_->worldTransform.rotate.x, 0.01f);
	ImGui::DragFloat3("Monkey Scale", &monkey_->worldTransform.scale.x, 0.01f);
	ImGui::DragFloat3("DogBlock Translate", &dogBlock_->worldTransform.translate.x, 0.01f);
	ImGui::DragFloat3("DogBlock Rotate", &dogBlock_->worldTransform.rotate.x, 0.01f);
	ImGui::DragFloat3("DogBlock Scale", &dogBlock_->worldTransform.scale.x, 0.01f);
	ImGui::DragFloat3("MonkeyBlock Translate", &monkeyBlock_->worldTransform.translate.x, 0.01f);
	ImGui::DragFloat3("MonkeyBlock Rotate", &monkeyBlock_->worldTransform.rotate.x, 0.01f);
	ImGui::DragFloat3("MonkeyBlock Scale", &monkeyBlock_->worldTransform.scale.x, 0.01f);
	ImGui::End();
#endif // _DEBUG
}

void TitleSystem::Finalize() {
}

void TitleSystem::DebugWithImGui() {
#ifdef _DEBUG
#endif // _DEBUG
}