#pragma once
#include <Input.h>
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

private://メンバ変数
	//インプット
	Input* input_ = nullptr;
	
	//背景
	std::unique_ptr<Sprite> backSprite_ = nullptr;
	uint32_t backTexture_ = 0u;
	//タイトル文字
	std::unique_ptr<Sprite> titleTextSprite_ = nullptr;
	Handle titleTextTexture_;
	//次へ進むUI
	std::unique_ptr<Sprite> nextUITextSprite_ = nullptr;
	Handle nextUITextTexture_;

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

