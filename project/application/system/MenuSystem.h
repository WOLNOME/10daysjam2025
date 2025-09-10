#pragma once
#include <Input.h>
#include <Audio.h>
#include <Sprite.h>
#include <Handle.h>
#include <memory>
#include "application/map/BackPlane.h"
class MenuSystem {
private://列挙体
	enum class SelectState {
		Play,		//プレイテキスト
		Tutorial,	//チュートリアルテキスト
	};

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
	//メニューテキスト
	std::unique_ptr<Sprite> menuTextSprite_ = nullptr;
	Handle menuTextTexture_;

	//選択状態
	SelectState selectState_ = SelectState::Play;
	//プレイテキスト
	std::unique_ptr<Sprite> playTextSprite_ = nullptr;
	Handle playTextTexture_;
	//チュートリアルテキスト
	std::unique_ptr<Sprite> tutorialTextSprite_ = nullptr;
	Handle tutorialTextTexture_;

	//シーン遷移中判定フラグ
	bool isSceneChanging_ = false;

	//移動音
	std::unique_ptr<Audio> moveSE_ = nullptr;
	//決定音
	std::unique_ptr<Audio> decideSE_ = nullptr;

	// 背景平面
	std::unique_ptr<BackPlane> backPlane_ = nullptr;

	// イヌ
	std::unique_ptr<Object3d> dog_ = nullptr;
	// サル
	std::unique_ptr<Object3d> monkey_ = nullptr;



};

