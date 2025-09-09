#pragma once
#include <Input.h>
#include <Handle.h>
#include <Sprite.h>
#include <TextTextureManager.h>
#include <string>
#include <list>
#include <memory>

class TextCreator {
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//終了
	void Finalize();
	//ImGuiデバッグ
	void DebugWithImGui();

	//テキストの追加
	void AddText(std::wstring _addText);

private://非公開メンバ関数
	//操作
	void Operator();
	//テキストの表示
	void ShowText();

private:
	//インプット
	Input* input_ = nullptr;

	//枠スプライト
	std::unique_ptr<Sprite> frame_ = nullptr;
	uint32_t frameTH_ = 0u;

	//テキスト
	std::unique_ptr<Sprite> text_ = nullptr;
	Handle textTH_;

	//テキストのリスト
	std::list<std::wstring> textLists_;

	//テキストのベース情報
	TextParam baseTextParam_;
	EdgeParam baseEdgeParam_;

	//テキスト表示するかフラグ
	bool isDisplayText_ = false;
};

