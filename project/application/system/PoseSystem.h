#pragma once
#include <Input.h>
#include <Audio.h>
#include <Sprite.h>
#include <Handle.h>
#include <memory>
class PoseSystem {
private://列挙体
	enum class SelectState {
		Restart,
		BackToStageSelect,
		BackToMenu,
	};

public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//終了
	void Finalize();
	//デバッグ処理
	void DebugWithImGui();

	//ポーズフラグ関連アクセッサ
	bool GetIsPosing() const { return isPosing_; }
	bool SetIsPosing(bool isPosing) { return isPosing_ = isPosing; }

private:
	//インプット
	Input* input_ = nullptr;

	//ポーズ中かフラグ
	bool isPosing_ = false;

	//背景スプライト
	std::unique_ptr<Sprite> backSprite_ = nullptr;
	uint32_t backTexture_ = 0u;
	//ポーズ中テキスト
	std::unique_ptr<Sprite> posingTextSprite_ = nullptr;
	Handle posingTextTexture_;

	//選択状態
	SelectState selectState_ = SelectState::Restart;
	//再開UIテキスト
	std::unique_ptr<Sprite> restartTextSprite_ = nullptr;
	Handle restartTextTexture_;
	//ステージセレクトに戻るUIテキスト
	std::unique_ptr<Sprite> backStageSelectTextSprite_ = nullptr;
	Handle backStageSelectTextTexture_;
	//メニューに戻るUIテキスト
	std::unique_ptr<Sprite> backMenuTextSprite_ = nullptr;
	Handle backMenuTextTexture_;

	//シーン遷移中判定フラグ
	bool isSceneChanging_ = false;

	//移動音
	std::unique_ptr<Audio> moveSE_ = nullptr;
	//決定音
	std::unique_ptr<Audio> decideSE_ = nullptr;

};

