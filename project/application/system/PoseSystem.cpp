#include "PoseSystem.h"
#include <SceneManager.h>
#include <SpriteManager.h>
#include <TextureManager.h>
#include <TextTextureManager.h>

void PoseSystem::Initialize() {
	//インプット
	input_ = Input::GetInstance();
	//背景スプライト生成
	backSprite_ = std::make_unique<Sprite>();
	backTexture_ = TextureManager::GetInstance()->LoadTexture("black.png");
	backSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("back"), Sprite::Order::Front1, backTexture_);
	backSprite_->SetPosition({ 0,0 });
	backSprite_->SetAnchorPoint({ 0,0 });
	backSprite_->SetColor({ 1,1,1,0.8f });
	backSprite_->SetIsDisplay(false);
	//ポーズ中テキストスプライト生成
	posingTextSprite_ = std::make_unique<Sprite>();
	TextParam posingTextParam;
	posingTextParam.text = L"- ポーズ中 -";
	posingTextParam.font = Font::UDDegitalN_B;
	posingTextParam.fontStyle = FontStyle::Normal;
	posingTextParam.size = 96.0f;
	posingTextParam.color = { 1.0f,1.0f,1.0f,1.0f };
	posingTextTexture_ = TextTextureManager::GetInstance()->LoadTextTexture(posingTextParam);
	posingTextSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("menu"), Sprite::Order::Front2);
	posingTextSprite_->SetTexture(posingTextTexture_);
	posingTextSprite_->SetPosition({ 640,120 });
	posingTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
	posingTextSprite_->SetIsDisplay(false);
	//再開UIテキストスプライト生成
	restartTextSprite_ = std::make_unique<Sprite>();
	TextParam restartTextParam;
	restartTextParam.text = L"再開する";
	restartTextParam.font = Font::UDDegitalN_B;
	restartTextParam.fontStyle = FontStyle::Normal;
	restartTextParam.size = 48.0f;
	restartTextParam.color = { 1.0f,1.0f,1.0f,1.0f };
	restartTextTexture_ = TextTextureManager::GetInstance()->LoadTextTexture(restartTextParam);
	restartTextSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("restart"), Sprite::Order::Front2);
	restartTextSprite_->SetTexture(restartTextTexture_);
	restartTextSprite_->SetPosition({ 640,240 });
	restartTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
	restartTextSprite_->SetIsDisplay(false);
	//ステージセレクトに戻るUIテキストスプライト生成
	backStageSelectTextSprite_ = std::make_unique<Sprite>();
	TextParam backStageSelectTextParam;
	backStageSelectTextParam.text = L"ステージ選択画面に戻る";
	backStageSelectTextParam.font = Font::UDDegitalN_B;
	backStageSelectTextParam.fontStyle = FontStyle::Normal;
	backStageSelectTextParam.size = 48.0f;
	backStageSelectTextParam.color = { 1.0f,1.0f,1.0f,1.0f };
	backStageSelectTextTexture_ = TextTextureManager::GetInstance()->LoadTextTexture(backStageSelectTextParam);
	backStageSelectTextSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("backStageSelect"), Sprite::Order::Front2);
	backStageSelectTextSprite_->SetTexture(backStageSelectTextTexture_);
	backStageSelectTextSprite_->SetPosition({ 640,360 });
	backStageSelectTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
	backStageSelectTextSprite_->SetIsDisplay(false);
	//メニューに戻るUIテキストスプライト生成
	backMenuTextSprite_ = std::make_unique<Sprite>();
	TextParam backMenuTextParam;
	backMenuTextParam.text = L"メニュー画面に戻る";
	backMenuTextParam.font = Font::UDDegitalN_B;
	backMenuTextParam.fontStyle = FontStyle::Normal;
	backMenuTextParam.size = 48.0f;
	backMenuTextParam.color = { 1.0f,1.0f,1.0f,1.0f };
	backMenuTextTexture_ = TextTextureManager::GetInstance()->LoadTextTexture(backMenuTextParam);
	backMenuTextSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("backMenu"), Sprite::Order::Front2);
	backMenuTextSprite_->SetTexture(backMenuTextTexture_);
	backMenuTextSprite_->SetPosition({ 640,480 });
	backMenuTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
	backMenuTextSprite_->SetIsDisplay(false);

	//移動音生成
	moveSE_ = std::make_unique<Audio>();
	moveSE_->Initialize("cursor.wav");
	//決定音生成
	decideSE_ = std::make_unique<Audio>();
	decideSE_->Initialize("decide.wav");

}

void PoseSystem::Update() {
	//もしもポーズ中でなければ
	if (!isPosing_) {
		//エスケープキーorスタートボタンでポーズ画面へ
		if (input_->TriggerKey(DIK_ESCAPE) || input_->TriggerPadButton(GamepadButton::Start)) {
			isPosing_ = true;
			//ポーズ中テキストとUIテキストを表示
			backSprite_->SetIsDisplay(true);
			posingTextSprite_->SetIsDisplay(true);
			restartTextSprite_->SetIsDisplay(true);
			backStageSelectTextSprite_->SetIsDisplay(true);
			backMenuTextSprite_->SetIsDisplay(true);
			//決定音再生
			decideSE_->Play();
		}
	}
	else {
		//上下キーで選択状態変更
		if ((input_->TriggerKey(DIK_W) || input_->TriggerKey(DIK_UP) || input_->TriggerPadButton(GamepadButton::DPadUp)) && !isSceneChanging_) {
			if (selectState_ == SelectState::BackToStageSelect) {
				selectState_ = SelectState::Restart;
				//移動音再生
				moveSE_->Play();
			}
			else if (selectState_ == SelectState::BackToMenu) {
				selectState_ = SelectState::BackToStageSelect;
				//移動音再生
				moveSE_->Play();
			}
		}
		if ((input_->TriggerKey(DIK_S) || input_->TriggerKey(DIK_DOWN) || input_->TriggerPadButton(GamepadButton::DPadDown)) && !isSceneChanging_) {
			if (selectState_ == SelectState::Restart) {
				selectState_ = SelectState::BackToStageSelect;
				//移動音再生
				moveSE_->Play();
			}
			else if (selectState_ == SelectState::BackToStageSelect) {
				selectState_ = SelectState::BackToMenu;
				//移動音再生
				moveSE_->Play();
			}
		}
		//選択状態に応じてテキストの色を変更
		switch (selectState_) {
		case SelectState::Restart:
			restartTextSprite_->SetColor({ 1.0f,1.0f,0.0f,1.0f });
			TextTextureManager::GetInstance()->EditTextSize(restartTextTexture_, 64.0f);
			backStageSelectTextSprite_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
			TextTextureManager::GetInstance()->EditTextSize(backStageSelectTextTexture_, 48.0f);
			break;
		case SelectState::BackToStageSelect:
			restartTextSprite_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
			TextTextureManager::GetInstance()->EditTextSize(restartTextTexture_, 48.0f);
			backStageSelectTextSprite_->SetColor({ 1.0f,1.0f,0.0f,1.0f });
			TextTextureManager::GetInstance()->EditTextSize(backStageSelectTextTexture_, 64.0f);
			backMenuTextSprite_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
			TextTextureManager::GetInstance()->EditTextSize(backMenuTextTexture_, 48.0f);
			break;
		case SelectState::BackToMenu:
			backStageSelectTextSprite_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
			TextTextureManager::GetInstance()->EditTextSize(backStageSelectTextTexture_, 48.0f);
			backMenuTextSprite_->SetColor({ 1.0f,1.0f,0.0f,1.0f });
			TextTextureManager::GetInstance()->EditTextSize(backMenuTextTexture_, 64.0f);
			break;
		}
		//スペースキーorAボタンで選択
		bool isRestart = false;
		if ((input_->TriggerKey(DIK_SPACE) || input_->TriggerPadButton(GamepadButton::ButtonA)) && !isSceneChanging_) {
			switch (selectState_) {
			case SelectState::Restart:
				isRestart = true;
				//決定音再生
				decideSE_->Play();
				break;
			case SelectState::BackToStageSelect:
				if (SceneManager::GetInstance()->SetNextScene("STAGESELECT")) {
					isSceneChanging_ = true;
					//決定音再生
					decideSE_->Play();
				}
				break;
			case SelectState::BackToMenu:
				if (SceneManager::GetInstance()->SetNextScene("MENU")) {
					isSceneChanging_ = true;
					//決定音再生
					decideSE_->Play();
				}
				break;
			}
		}
		//エスケープキーorBボタンもポーズを閉じられる
		if ((input_->TriggerKey(DIK_ESCAPE) || input_->TriggerPadButton(GamepadButton::ButtonB)) && !isSceneChanging_) {
			isRestart = true;
			//決定音再生
			decideSE_->Play();
		}
		//ポーズを閉じる処理
		if (isRestart) {
			isPosing_ = false;
			//ポーズ中テキストとUIテキストを非表示
			backSprite_->SetIsDisplay(false);
			posingTextSprite_->SetIsDisplay(false);
			restartTextSprite_->SetIsDisplay(false);
			backStageSelectTextSprite_->SetIsDisplay(false);
			backMenuTextSprite_->SetIsDisplay(false);
		}
	}
}

void PoseSystem::Finalize() {
}

void PoseSystem::DebugWithImGui() {
#ifdef _DEBUG

#endif // _DEBUG

}
