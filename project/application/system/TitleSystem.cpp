#include "TitleSystem.h"
#include <SceneManager.h>
#include <SpriteManager.h>
#include <TextureManager.h>
#include <TextTextureManager.h>

void TitleSystem::Initialize() {
	//インプット
	input_ = Input::GetInstance();

	//背景スプライト生成
	backSprite_ = std::make_unique<Sprite>();
	backTexture_ = TextureManager::GetInstance()->LoadTexture("black.png");
	backSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("back"), Sprite::Order::Back0, backTexture_);
	backSprite_->SetPosition({ 0,0 });
	backSprite_->SetAnchorPoint({ 0,0 });
	//タイトル文字スプライト生成
	titleTextSprite_ = std::make_unique<Sprite>();
	TextParam titleTextParam;
	titleTextParam.text = L"犬猿";
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

}

void TitleSystem::Update() {
	//スペースキーorBボタンで次のシーンへ
	if ((input_->TriggerKey(DIK_SPACE) || input_->TriggerPadButton(GamepadButton::ButtonA)) && !isSceneChanging_) {
		SceneManager::GetInstance()->SetNextScene("MENU");
		isSceneChanging_ = true;
		decideSE_->Play();
	}
}

void TitleSystem::Finalize() {
}

void TitleSystem::DebugWithImGui() {
#ifdef _DEBUG
#endif // _DEBUG
}