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

	//移動音生成
	moveSE_ = std::make_unique<Audio>();
	moveSE_->Initialize("cursor.wav");
	//決定音生成
	decideSE_ = std::make_unique<Audio>();
	decideSE_->Initialize("decide.wav");
}

void StageSelectSystem::Update() {
	//スペースキーorAボタンで次のシーンへ(後々選択できるようにする)
	if ((input_->TriggerKey(DIK_SPACE) || input_->TriggerPadButton(GamepadButton::ButtonA)) && !isSceneChanging_) {
		if (SceneManager::GetInstance()->SetNextScene("GAMEPLAY")) {
			isSceneChanging_ = true;
			//決定音再生
			decideSE_->Play();
		}
	}
	//エスケープキーorBボタンで前のシーンへ
	if ((input_->TriggerKey(DIK_ESCAPE) || input_->TriggerPadButton(GamepadButton::ButtonB)) && !isSceneChanging_) {
		if (SceneManager::GetInstance()->SetNextScene("MENU")) {
			isSceneChanging_ = true;
			//決定音再生
			decideSE_->Play();
		}
	}

}

void StageSelectSystem::Finalize() {
}

void StageSelectSystem::DebugWithImGui() {
#ifdef _DEBUG
#endif // _DEBUG
}
