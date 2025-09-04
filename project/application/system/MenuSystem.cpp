#include "MenuSystem.h"
#include <SceneManager.h>
#include <SpriteManager.h>
#include <TextureManager.h>
#include <TextTextureManager.h>

void MenuSystem::Initialize() {
	//インプット
	input_ = Input::GetInstance();
	//背景スプライト生成
	backSprite_ = std::make_unique<Sprite>();
	backTexture_ = TextureManager::GetInstance()->LoadTexture("black.png");
	backSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("back"), Sprite::Order::Back0, backTexture_);
	backSprite_->SetPosition({ 0,0 });
	backSprite_->SetAnchorPoint({ 0,0 });
	//メニューテキストスプライト生成
	menuTextSprite_ = std::make_unique<Sprite>();
	TextParam menuTextParam;
	menuTextParam.text = L"メニュー";
	menuTextParam.font = Font::UDDegitalN_B;
	menuTextParam.fontStyle = FontStyle::Normal;
	menuTextParam.size = 96.0f;
	menuTextParam.color = { 1.0f,1.0f,1.0f,1.0f };
	menuTextTexture_ = TextTextureManager::GetInstance()->LoadTextTexture(menuTextParam);
	menuTextSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("menu"), Sprite::Order::Back1);
	menuTextSprite_->SetTexture(menuTextTexture_);
	menuTextSprite_->SetPosition({ 640,120 });
	menuTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
	//プレイテキストスプライト生成
	playTextSprite_ = std::make_unique<Sprite>();
	TextParam playTextParam;
	playTextParam.text = L"プレイ";
	playTextParam.font = Font::UDDegitalN_B;
	playTextParam.fontStyle = FontStyle::Normal;
	playTextParam.size = 48.0f;
	playTextParam.color = { 1.0f,1.0f,1.0f,1.0f };
	playTextTexture_ = TextTextureManager::GetInstance()->LoadTextTexture(playTextParam);
	playTextSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("play"), Sprite::Order::Back1);
	playTextSprite_->SetTexture(playTextTexture_);
	playTextSprite_->SetPosition({ 640,240 });
	playTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
	//チュートリアルテキストスプライト生成
	tutorialTextSprite_ = std::make_unique<Sprite>();
	TextParam tutorialTextParam;
	tutorialTextParam.text = L"チュートリアル";
	tutorialTextParam.font = Font::UDDegitalN_B;
	tutorialTextParam.fontStyle = FontStyle::Normal;
	tutorialTextParam.size = 48.0f;
	tutorialTextParam.color = { 1.0f,1.0f,1.0f,1.0f };
	tutorialTextTexture_ = TextTextureManager::GetInstance()->LoadTextTexture(tutorialTextParam);
	tutorialTextSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("tutorial"), Sprite::Order::Back1);
	tutorialTextSprite_->SetTexture(tutorialTextTexture_);
	tutorialTextSprite_->SetPosition({ 640,360 });
	tutorialTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
}

void MenuSystem::Update() {
	//上下キーで選択状態変更
	if (input_->TriggerKey(DIK_W) || input_->TriggerKey(DIK_UP) || input_->TriggerPadButton(GamepadButton::DPadUp)) {
		if (selectState_ == SelectState::Tutorial) {
			selectState_ = SelectState::Play;
		}
	}
	if (input_->TriggerKey(DIK_S) || input_->TriggerKey(DIK_DOWN) || input_->TriggerPadButton(GamepadButton::DPadDown)) {
		if (selectState_ == SelectState::Play) {
			selectState_ = SelectState::Tutorial;
		}
	}
	//選択状態に応じてテキストの色を変更
	switch (selectState_) {
	case SelectState::Play:
		playTextSprite_->SetColor({ 1.0f,1.0f,0.0f,1.0f });
		TextTextureManager::GetInstance()->EditTextSize(playTextTexture_, 64.0f);
		tutorialTextSprite_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		TextTextureManager::GetInstance()->EditTextSize(tutorialTextTexture_, 48.0f);
		break;
	case SelectState::Tutorial:
		playTextSprite_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		TextTextureManager::GetInstance()->EditTextSize(playTextTexture_, 48.0f);
		tutorialTextSprite_->SetColor({ 1.0f,1.0f,0.0f,1.0f });
		TextTextureManager::GetInstance()->EditTextSize(tutorialTextTexture_, 64.0f);
		break;
	}
	//スペースキーorAボタンで次のシーンへ
	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerPadButton(GamepadButton::ButtonA)) {
		switch (selectState_) {
		case SelectState::Play:
			SceneManager::GetInstance()->SetNextScene("STAGESELECT");
			break;
		case SelectState::Tutorial:
			SceneManager::GetInstance()->SetNextScene("TUTORIAL");
			break;
		}
	}
	//エスケープキーorBボタンで前のシーンへ
	if (input_->TriggerKey(DIK_ESCAPE) || input_->TriggerPadButton(GamepadButton::ButtonB)) {
		SceneManager::GetInstance()->SetNextScene("TITLE");
	}
}

void MenuSystem::DebugWithImGui() {
#ifdef _DEBUG

#endif // _DEBUG

}