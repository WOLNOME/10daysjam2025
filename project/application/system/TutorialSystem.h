#pragma once
#include <Input.h>
#include <string>
#include <Sprite.h>
#include <Handle.h>
#include <memory>

class TutorialSystem {
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//ImGuiデバッグ
	void DebugWithImGui();


private:
	//テキストリスト
	std::vector<std::wstring> textList_;
	int currentIndex_ = 0;

	//テキストスプライト
	std::unique_ptr<Sprite> textSprite_ = nullptr;
	Handle textTexture_;

	//スペースキー入力
	std::unique_ptr<Sprite> spaceSprite_ = nullptr;
	Handle spaceTexture_;
	const float time_ = 1.0f;
	float timer_ = 0.0f;
	bool isAlphaUp_ = false;

};

