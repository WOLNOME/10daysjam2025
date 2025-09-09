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
	backTexture_ = TextureManager::GetInstance()->LoadTexture("souko.png");
	backSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("back"), Sprite::Order::Back0, backTexture_);
	backSprite_->SetPosition({ 0,0 });
	backSprite_->SetSize({ 1280.0f,720.0f });
	backSprite_->SetAnchorPoint({ 0,0 });
	//犬画像スプライト生成
	dogSprite_ = std::make_unique<Sprite>();
	dogTexture_ = TextureManager::GetInstance()->LoadTexture("dog_real.png");
	dogSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("dog"), Sprite::Order::Back1,dogTexture_);
	dogSprite_->SetSize({ 380.0f,380.0f });
	dogSprite_->SetPosition({ 200,530 });
	dogSprite_->SetAnchorPoint({ 0.5f,0.5f });
	//サル画像スプライト生成
	monkeySprite_ = std::make_unique<Sprite>();
	monkeyTexture_ = TextureManager::GetInstance()->LoadTexture("monkey_real.png");
	monkeySprite_->Initialize(SpriteManager::GetInstance()->GenerateName("monkey"), Sprite::Order::Back1,monkeyTexture_);
	monkeySprite_->SetSize({ 380.0f,380.0f });
	monkeySprite_->SetPosition({ 1000,520 });
	monkeySprite_->SetAnchorPoint({ 0.5f,0.5f });

	//タイトル文字スプライト生成
	titleTextSprite_ = std::make_unique<Sprite>();
	TextParam titleTextParam;
	titleTextParam.text = L"すれ違い協定\n~和解の出口~";
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
		if (SceneManager::GetInstance()->SetNextScene("MENU")) {
			isSceneChanging_ = true;
			//決定音再生
			decideSE_->Play();
		}
	}

	//UIのアルファ値更新
	UpdateUI();
}

void TitleSystem::Finalize() {
}

void TitleSystem::DebugWithImGui() {
#ifdef _DEBUG
#endif // _DEBUG
}

void TitleSystem::UpdateUI() {
	//タイトル文字の落下アニメーション
	{
		if (isFalling_) {
			fallTimer_ += kDeltaTime;
			if (fallTimer_ >= fallTime_) {
				fallTimer_ = fallTime_;
				isFalling_ = false;
			}
			const float posY = MyMath::Lerp(-200.0f, 180.0f, MyMath::EaseOutBounce(fallTimer_ / fallTime_));
			titleTextSprite_->SetPosition({ 640.0f,posY });
		}
	}

	//次へ進むUIのアルファ値更新
	{
		oneWayTimer_ += kDeltaTime;
		if (oneWayTimer_ >= oneWayTime_) {
			oneWayTimer_ = 0.0f;
			isAlphaUp_ = !isAlphaUp_;
		}

		if (!isAlphaUp_) {
			const float alpha = MyMath::Lerp(1.0f, 0.0f, MyMath::EaseInSine(oneWayTimer_ / oneWayTime_));
			nextUITextSprite_->SetColor({ 1.0f,1.0f,1.0f,alpha });
		}
		else {
			const float alpha = MyMath::Lerp(0.0f, 1.0f, MyMath::EaseInSine(oneWayTimer_ / oneWayTime_));
			nextUITextSprite_->SetColor({ 1.0f,1.0f,1.0f,alpha });
		}
	}

	//犬とサルのジャンプアニメーション
	{
		if (isJumping_) {
			jumpTimer_ += kDeltaTime;
			if (jumpTimer_ >= jumpTime_) {
				jumpTimer_ = jumpTime_;
				isJumping_ = false;
			}
			const float posY = MyMath::Lerp(530.0f, 480.0f, MyMath::EaseOutSine(jumpTimer_ / jumpTime_));
			dogSprite_->SetPosition({ 200.0f,posY });
			monkeySprite_->SetPosition({ 1000.0f,posY });
		}
		else {
			jumpTimer_ -= kDeltaTime;
			if (jumpTimer_ <= 0.0f) {
				jumpTimer_ = 0.0f;
				isJumping_ = true;
			}
			const float posY = MyMath::Lerp(530.0f, 480.0f, MyMath::EaseOutSine(jumpTimer_ / jumpTime_));
			dogSprite_->SetPosition({ 200.0f,posY });
			monkeySprite_->SetPosition({ 1000.0f,posY });
		}
	}

}
