#include "StageSelectSystem.h"
#include <SceneManager.h>
#include <SpriteManager.h>
#include <TextureManager.h>
#include <TextTextureManager.h>
#include <string>
#include <application/scene/GamePlayScene.h>

void StageSelectSystem::Initialize() {
	//インプット
	input_ = Input::GetInstance();
	//背景スプライト生成
	/*backSprite_ = std::make_unique<Sprite>();
	backTexture_ = TextureManager::GetInstance()->LoadTexture("black.png");
	backSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("back"), Sprite::Order::Back0, backTexture_);
	backSprite_->SetPosition({ 0,0 });
	backSprite_->SetAnchorPoint({ 0,0 });*/
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

	// ステージカード生成
	LoadStageTextures();
	LayoutStageSprites();

	selectedIndex_ = 0;
	viewIndex_ = static_cast<float>(selectedIndex_);
	tweening_ = false;


	//移動音生成
	moveSE_ = std::make_unique<Audio>();
	moveSE_->Initialize("cursor.wav");
	//決定音生成
	decideSE_ = std::make_unique<Audio>();
	decideSE_->Initialize("decide.wav");

	//背景
	backPlane_ = std::make_unique<BackPlane>();
	backPlane_->Initialize();

}

void StageSelectSystem::Update() {


	// A/D（または十字キー）での移動処理（長押し加速含む）
	HandleMoveInput();

	// 補間を進める（EaseOutCubic）
	if (tweening_) {
		tweenTime_ += kDeltaTime; // MyMath の定数
		float t = tweenTime_ / tweenDuration_;
		if (t >= 1.0f) {
			t = 1.0f;
			tweening_ = false;
		}
		float eased = MyMath::EaseOutCubic(t);
		viewIndex_ = MyMath::Lerp(tweenFrom_, static_cast<float>(selectedIndex_), eased);
		LayoutStageSprites();
	}

	if ((input_->TriggerKey(DIK_SPACE) || input_->TriggerPadButton(GamepadButton::ButtonA)) && !isSceneChanging_) {
		// 1-based のステージ名を作る（選択 0 → "Stage_1"）
		const int stageNo = selectedIndex_ + 1;
		std::string stageName = "Stage_" + std::to_string(stageNo);

		// GamePlayScene へ渡す
		GamePlayScene::SetPendingStageName(stageName);

		// シーン遷移
		if (SceneManager::GetInstance()->SetNextScene("GAMEPLAY")) {
			isSceneChanging_ = true;
			decideSE_->Play();
		}
	}

	////スペースキーorAボタンで次のシーンへ(後々選択できるようにする)
	//if ((input_->TriggerKey(DIK_SPACE) || input_->TriggerPadButton(GamepadButton::ButtonA)) && !isSceneChanging_) {
	//	if (SceneManager::GetInstance()->SetNextScene("GAMEPLAY")) {
	//		isSceneChanging_ = true;
	//		//決定音再生
	//		decideSE_->Play();
	//	}
	//}
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

void StageSelectSystem::LoadStageTextures()
{
	stageSprites_.resize(kStageCount_);
	stageTextures_.resize(kStageCount_);
	baseSizes_.resize(kStageCount_);

	for (int i = 0; i < kStageCount_; ++i) {
		char path[64];
		std::snprintf(path, sizeof(path), "number/%d.png", i + 1);

		uint32_t tex = TextureManager::GetInstance()->LoadTexture(path);
		stageTextures_[i] = tex;

		auto spr = std::make_unique<Sprite>();
		spr->Initialize(
			SpriteManager::GetInstance()->GenerateName(("stage_" + std::to_string(i + 1)).c_str()),
			Sprite::Order::Front0,
			tex
		);
		spr->SetAnchorPoint({ 0.5f, 0.5f });

		// 生成直後のサイズを取得して保存（以降は倍率×でSetSize）
		baseSizes_[i] = spr->GetSize();

		stageSprites_[i] = std::move(spr);
	}
}

void StageSelectSystem::LayoutStageSprites()
{
	const float centerX = 640.0f;
	for (int i = 0; i < kStageCount_; ++i) {
		float x = centerX + (static_cast<float>(i) - viewIndex_) * spacing_;
		stageSprites_[i]->SetPosition({ x, yPos_ });

		const float mul = (i == selectedIndex_) ? (scale_ * focusMul_) : scale_;

		const Vector2 base = baseSizes_[i];
		stageSprites_[i]->SetSize({ base.x * mul, base.y * mul });

		// 見栄えの色だけ調整
		if (i == selectedIndex_) {
			stageSprites_[i]->SetColor({ 1,1,1,1 });
		}
		else {
			stageSprites_[i]->SetColor({ 0.8f,0.8f,0.8f,0.9f });
		}
	}
}

void StageSelectSystem::StepLeft()
{
	if (selectedIndex_ > 0) {
		selectedIndex_--;
		moveSE_->Play();
		StartTween();
		//LayoutStageSprites();
	}
}

void StageSelectSystem::StepRight()
{
	if (selectedIndex_ < kStageCount_ - 1) {
		selectedIndex_++;
		moveSE_->Play();
		StartTween();
		//LayoutStageSprites();
	}
}

void StageSelectSystem::HandleMoveInput()
{

	const bool leftTrig = input_->TriggerKey(DIK_A) || input_->TriggerPadButton(GamepadButton::DPadLeft);
	const bool rightTrig = input_->TriggerKey(DIK_D) || input_->TriggerPadButton(GamepadButton::DPadRight);
	const bool leftHold = input_->PushKey(DIK_A) || input_->PushPadButton(GamepadButton::DPadLeft);
	const bool rightHold = input_->PushKey(DIK_D) || input_->PushPadButton(GamepadButton::DPadRight);

	// 単発
	if (leftTrig) { StepLeft();  holdLeftFrames_ = 0; }
	if (rightTrig) { StepRight(); holdRightFrames_ = 0; }

	// 長押し（リピート）
	// 左
	if (leftHold && !rightHold) {
		holdLeftFrames_++;
		if (holdLeftFrames_ >= initialDelayFrames_) {
			const bool accelerated = (holdLeftFrames_ >= acceleratedThreshold_);
			int interval = accelerated ? fastIntervalFrames_ : repeatIntervalFrames_;
			if ((holdLeftFrames_ - initialDelayFrames_) % interval == 0) {
				StepLeft();
			}
		}
	}
	else {
		holdLeftFrames_ = 0;
	}

	// 右
	if (rightHold && !leftHold) {
		holdRightFrames_++;
		if (holdRightFrames_ >= initialDelayFrames_) {
			const bool accelerated = (holdRightFrames_ >= acceleratedThreshold_);
			int interval = accelerated ? fastIntervalFrames_ : repeatIntervalFrames_;
			if ((holdRightFrames_ - initialDelayFrames_) % interval == 0) {
				StepRight();
			}
		}
	}
	else {
		holdRightFrames_ = 0;
	}
}

void StageSelectSystem::StartTween()
{
	tweenFrom_ = viewIndex_;
	tweenTime_ = 0.0f;
	tweening_ = true;
}
