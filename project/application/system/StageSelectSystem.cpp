#include "StageSelectSystem.h"
#include <SceneManager.h>
#include <SpriteManager.h>
#include <TextureManager.h>
#include <TextTextureManager.h>

void StageSelectSystem::Initialize() {
	//インプット
	input_ = Input::GetInstance();
	//背景スプライト生成
	backSprite_ = std::make_unique<Sprite>();
	backTexture_ = TextureManager::GetInstance()->LoadTexture("black.png");
	backSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("back"), Sprite::Order::Back0, backTexture_);
	backSprite_->SetPosition({ 0,0 });
	backSprite_->SetAnchorPoint({ 0,0 });
	//セレクトテキストスプライト生成
	menuTextSprite_ = std::make_unique<Sprite>();
	TextParam menuTextParam;
	menuTextParam.text = L"ステージセレクト";
	menuTextParam.font = Font::UDDegitalN_B;
	menuTextParam.fontStyle = FontStyle::Normal;
	menuTextParam.size = 96.0f;
	menuTextParam.color = { 1.0f,1.0f,1.0f,1.0f };
	menuTextTexture_ = TextTextureManager::GetInstance()->LoadTextTexture(menuTextParam);
	menuTextSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("select"), Sprite::Order::Back1);
	menuTextSprite_->SetTexture(menuTextTexture_);
	menuTextSprite_->SetPosition({ 640,120 });
	menuTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
}

void StageSelectSystem::Update() {
	//スペースキーorAボタンで次のシーンへ(後々選択できるようにする)
	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerPadButton(GamepadButton::ButtonA)) {
		SceneManager::GetInstance()->SetNextScene("GAMEPLAY");
	}
	//エスケープキーorBボタンで前のシーンへ
	if (input_->TriggerKey(DIK_ESCAPE) || input_->TriggerPadButton(GamepadButton::ButtonB)) {
		SceneManager::GetInstance()->SetNextScene("MENU");
	}

}

void StageSelectSystem::Finalize() {
}

void StageSelectSystem::DebugWithImGui() {
#ifdef _DEBUG
#endif // _DEBUG
}
