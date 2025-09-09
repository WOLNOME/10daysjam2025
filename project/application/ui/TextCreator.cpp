#include "TextCreator.h"
#include <SpriteManager.h>
#include <TextureManager.h>

void TextCreator::Initialize() {
	//インプット
	input_ = Input::GetInstance();

	//枠スプライト
	frameTH_ = TextureManager::GetInstance()->LoadTexture("frame.png");
	frame_ = std::make_unique<Sprite>();
	frame_->Initialize(SpriteManager::GetInstance()->GenerateName("frame"), Sprite::Order::Front0, frameTH_);
	frame_->SetAnchorPoint({ 0.5f,0.5f });
	frame_->SetPosition({ 640.0f,550.0f });
	frame_->SetIsDisplay(false);

	//テキストスプライト
	text_ = std::make_unique<Sprite>();
	text_->Initialize(SpriteManager::GetInstance()->GenerateName("text"), Sprite::Order::Front1);
	text_->SetAnchorPoint({ 0.5f,0.5f });
	text_->SetPosition({ 640.0f,550.0f });
	text_->SetIsDisplay(false);

	//ベースのテキストパラメーター
	baseTextParam_.color = { 1.0f,1.0f,1.0f,1.0f };
	baseTextParam_.font = Font::UDDegitalNK_R;
	baseTextParam_.fontStyle = FontStyle::Normal;
	baseTextParam_.size = 24.0f;
	//ベースのエッジパラメーター
	baseEdgeParam_.color = { 0.0f,0.0f,0.0f,1.0f };
	baseEdgeParam_.isEdgeDisplay = true;
	baseEdgeParam_.slideRate = { 0.0f,0.0f };
	baseEdgeParam_.width = 2.0f;

}

void TextCreator::Update() {
	//操作
	Operator();
	//テキストの表示
	ShowText();

}

void TextCreator::Finalize() {

}

void TextCreator::DebugWithImGui() {

}

void TextCreator::AddText(std::wstring _addText) {
	//リストに保存
	textLists_.push_back(_addText);
}

void TextCreator::Operator() {
	//文章表示中の時
	if(isDisplayText_) {
		//文章書き込み中の時
		if (isWritingText_) {
			//スペースキーorAボタンで文章スキップ
			if (input_->TriggerKey(DIK_SPACE) || input_->TriggerPadButton(GamepadButton::ButtonA)) {
				isSkipText_ = true;
			}
		}
		//文章書き終わっているとき
		else if (isFinishedText_) {
			//スペースキーorAボタンで次の文章へ
			if (input_->TriggerKey(DIK_SPACE) || input_->TriggerPadButton(GamepadButton::ButtonA)) {
				isNextText_ = true;
			}
		}
	}
}

void TextCreator::ShowText() {
	//リストに文字が追加されたら枠を表示
	if (!isDisplayText_ && !textLists_.empty()) {
		frameDisplayTimer_ += kDeltaTime;
		frame_
		float alpha=MyMath::Lerp(0.0f,1.0f)
	}

}

