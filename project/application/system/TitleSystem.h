#pragma once
#include <Input.h>
#include <Audio.h>
#include <Sprite.h>
#include <Handle.h>
#include <memory>
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

};

