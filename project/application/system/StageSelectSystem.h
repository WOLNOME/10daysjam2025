#pragma once
#include <Input.h>
#include <Audio.h>
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

	//シーン遷移中判定フラグ
	bool isSceneChanging_ = false;

	//移動音
	std::unique_ptr<Audio> moveSE_ = nullptr;
	//決定音
	std::unique_ptr<Audio> decideSE_ = nullptr;


};

