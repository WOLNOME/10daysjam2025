#pragma once
#include "BaseScene.h"
#include "GameCamera.h"
#include <memory>

//アプリケーション
#include <application/system/PoseSystem.h>

class TutorialScene : public BaseScene {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 終了時
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// デバッグ処理
	/// </summary>
	void DebugWithImGui() override;

private://メンバ変数
	Input* input_ = nullptr;
	//開発用カメラ
	std::unique_ptr<GameCamera> camera_ = nullptr;

	//ポーズシステム
	std::unique_ptr<PoseSystem> poseSystem_ = nullptr;
};

