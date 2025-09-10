#pragma once
#include <Input.h>
#include <MyMath.h>
#include <Audio.h>
#include <Sprite.h>
#include <Handle.h>
#include <memory>

#include "engine/3d/DevelopCamera.h"
#include "application/map/BackPlane.h"
class TitleSystem {
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//終了
	void Finalize();
	//ImGuiデバッグ
	void DebugWithImGui();

private://非公開メンバ関数
	void UpdateUI();

private://メンバ変数
	//インプット
	Input* input_ = nullptr;
	
	//背景
	std::unique_ptr<Sprite> backSprite_ = nullptr;
	uint32_t backTexture_ = 0u;
	//犬画像
	std::unique_ptr<Sprite> dogSprite_ = nullptr;
	uint32_t dogTexture_;
	//サル画像
	std::unique_ptr<Sprite> monkeySprite_ = nullptr;
	uint32_t monkeyTexture_;
	const float jumpTime_ = 0.3f;
	float jumpTimer_ = 0.0f;
	bool isJumping_ = false;

	//タイトル文字
	std::unique_ptr<Sprite> titleTextSprite_ = nullptr;
	Handle titleTextTexture_;
	const float fallTime_ = 2.0f;
	float fallTimer_ = 0.0f;
	bool isFalling_ = true;
	//次へ進むUI
	std::unique_ptr<Sprite> nextUITextSprite_ = nullptr;
	Handle nextUITextTexture_;
	const float oneWayTime_ = 2.0f;
	float oneWayTimer_ = 0.0f;
	bool isAlphaUp_ = true;

	//シーン遷移中判定フラグ
	bool isSceneChanging_ = false;

	//決定音
	std::unique_ptr<Audio> decideSE_ = nullptr;

	// 背景平面
	std::unique_ptr<BackPlane> backPlane_ = nullptr;

	// イヌ
	std::unique_ptr<Object3d> dog_ = nullptr;
	// サル
	std::unique_ptr<Object3d> monkey_ = nullptr;

	// 犬ブロック
	std::unique_ptr<Object3d> dogBlock_ = nullptr;
	// 猿ブロック
	std::unique_ptr<Object3d> monkeyBlock_ = nullptr;

	float dogBaseY_ = 0.0f;
	float dogTime_ = 0.0f;

};

