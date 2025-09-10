#pragma once
#include <Input.h>
#include <Audio.h>
#include <Sprite.h>
#include <Handle.h>
#include <memory>
#include "MyMath.h"
#include <application/map/BackPlane.h>

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

private:
	// ==== 内部ユーティリティ ====
	void LoadStageTextures();     // 30枚のステージカードを読み込み＆スプライト生成
	void LayoutStageSprites();    // 選択中インデックスに合わせて横並びレイアウト
	void StepLeft();              // 一つ左へ
	void StepRight();             // 一つ右へ
	void HandleMoveInput();       // A/Dの単発＋長押しリピート処理
	void StartTween();            // 補間開始

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


	// ==== ステージ選択UI ====
	static constexpr int kStageCount_ = 20;
	int selectedIndex_ = 0;                 // 0..29
	float yPos_ = 600.0f;                   // 1280x720 の画面で下寄せに表示
	float spacing_ = 420.0f;                // 等間隔ピッチ（画面例のように3つくらい見える間隔）
	float scale_ = 1.5f;                    // 必要ならカード縮尺
	float focusMul_ = 2.0f;       // 選択中だけ少し拡大

	std::vector<std::unique_ptr<Sprite>> stageSprites_; // 各ステージのカード
	std::vector<uint32_t> stageTextures_;               // テクスチャID保持
	std::vector<Vector2>  baseSizes_;      // 生成直後の元サイズを保持

	// ステージのプレビュー画像
	std::vector<std::unique_ptr<Sprite>> stagePreviewSprites_;
	std::vector<uint32_t> stagePreviewTextures_;
	std::vector<Vector2>  stagePreviewBaseSizes_;

	float previewYOffset_ = 250.0f; // 番号より上に出すオフセット（px）
	float previewScale_ = 0.25f;   // プレビューの通常倍率
	float previewFocusMul_ = 1.2f;   // 選択中だけ少し大きく

	// イージング補間
	float viewIndex_ = 0.0f;   // 実表示のインデックス(小数)
	float tweenFrom_ = 0.0f;   // 補間開始時の viewIndex_
	float tweenTime_ = 0.0f;   // 経過時間
	float tweenDuration_ = 0.7f;  // 1コマ移動の所要時間
	bool  tweening_ = false;

	// ==== 入力リピート（長押し加速） ====
	// フレームベースで実装（60fps想定）※あなたのInputに Push/Press が無い場合は関数名を合わせてください
	int holdLeftFrames_ = 0;
	int holdRightFrames_ = 0;
	// 初回ディレイ → 通常リピート → 加速リピート
	int initialDelayFrames_ = 18;   // 0.3s
	int repeatIntervalFrames_ = 5;  // 0.083s
	int fastIntervalFrames_ = 2;    // 0.033s
	int acceleratedThreshold_ = 45; // 0.75s 以上で加速


	//背景
	std::unique_ptr<BackPlane> backPlane_ = nullptr;

};

