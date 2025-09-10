#include "TutorialSystem.h"
#include <TextureManager.h>
#include <TextTextureManager.h>

void TutorialSystem::Initialize() {
	//テキストリストの初期化
	textList_.push_back(L"操作方法");
	textList_.push_back(L"WASD：移動");
	textList_.push_back(L"Qキー：犬を操作 Eキー:猿を操作");
	textList_.push_back(L"Rキー：リセット");
	textList_.push_back(L"Zキー：ひとつ戻る Yキー:ひとつ進む");
	textList_.push_back(L"Escapeキー：ポーズ");
	textList_.push_back(L"犬と猿を交互に操作して、ゴールを目指そう！");
	textList_.push_back(L"犬は箱を押すことができるよ！");
	textList_.push_back(L"猿はスイッチを押すことできるよ！");
	textList_.push_back(L"ツートーンの床に犬と猿が入ったらクリアだよ！");
	//テキストスプライトの初期化
	textSprite_ = std::make_unique<Sprite>();
	textSprite_->Initialize("SETSUMEI", Sprite::Order::Front0);
	textSprite_->SetPosition({ 640.0f,550.0f });
	textSprite_->SetAnchorPoint({ 0.5f,0.5f });
	TextParam param{};
	param.font = Font::UDDegitalN_B;
	param.fontStyle = FontStyle::Normal;
	param.size = 30.0f;
	param.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	param.text = textList_[currentIndex_];
	textTexture_ = TextTextureManager::GetInstance()->LoadTextTexture(param);
	textSprite_->SetTexture(textTexture_);
	textSprite_->SetIsDisplay(true);
	//スペースキー入力の初期化
	spaceSprite_ = std::make_unique<Sprite>();
	spaceSprite_->Initialize("SPACE", Sprite::Order::Front0);
	spaceSprite_->SetPosition({ 640.0f, 600.0f });
	spaceSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	TextParam spaceParam{};
	spaceParam.font = Font::UDDegitalN_R;
	spaceParam.fontStyle = FontStyle::Normal;
	spaceParam.size = 20.0f;
	spaceParam.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	spaceParam.text = L"[SPACE]で次へ";
	spaceTexture_ = TextTextureManager::GetInstance()->LoadTextTexture(spaceParam);
	spaceSprite_->SetTexture(spaceTexture_);
	spaceSprite_->SetIsDisplay(true);
	
}

void TutorialSystem::Update() {
	//スペースキー入力
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		//テキストの更新
		currentIndex_++;
		//もしテキストリストの範囲外にでたら
		if (currentIndex_ >= static_cast<int>(textList_.size())) {
			//テキストスプライトとスペーススプライトを非表示にする
			textSprite_->SetIsDisplay(false);
			spaceSprite_->SetIsDisplay(false);
			currentIndex_ = static_cast<int>(textList_.size()) - 1;
			return;
		}
		//テキストパラメーターの更新
		TextParam param{};
		param.font = Font::UDDegitalN_B;
		param.fontStyle = FontStyle::Normal;
		param.size = 30.0f;
		param.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		param.text = textList_[currentIndex_];
		textTexture_ = TextTextureManager::GetInstance()->LoadTextTexture(param);
		textSprite_->SetTexture(textTexture_);
	}

	//スペースキー入力を点滅
	timer_ += kDeltaTime;
	if (timer_ >= time_) {
		timer_ = 0.0f;
		isAlphaUp_ = !isAlphaUp_;
	}
	if (isAlphaUp_) {
		float alpha = MyMath::Lerp(0.0f, 1.0f, MyMath::EaseInOutSine(timer_/time_));
		spaceSprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha });
	}
	else {
		float alpha = MyMath::Lerp(1.0f, 0.0f, MyMath::EaseInOutSine(timer_ / time_));
		spaceSprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha });
	}
}

void TutorialSystem::DebugWithImGui() {
}