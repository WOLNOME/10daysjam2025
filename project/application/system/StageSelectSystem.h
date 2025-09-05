#pragma once
#include <Input.h>
#include <Sprite.h>
#include <Handle.h>
#include <memory>
class StageSelectSystem {
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
	//セレクトテキスト
	std::unique_ptr<Sprite> menuTextSprite_ = nullptr;
	Handle menuTextTexture_;

};

