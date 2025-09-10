#pragma once
#include "BaseScene.h"
#include "Audio.h"
#include "GameCamera.h"
#include "engine/3d/DevelopCamera.h"


#include <memory>
#include <DevelopCamera.h>
#include <string>

//アプリケーション
#include <application/system/PoseSystem.h>
#include "application/map/Map.h"
#include "application/character/Player.h"

class GamePlayScene : public BaseScene {
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

	static void SetPendingStageName(const std::string& name) { pendingStageName_ = name; }

private://メンバ変数
	Input* input_ = nullptr;
	//開発用カメラ
	std::unique_ptr<DevelopCamera> camera_ = nullptr;

	//std::unique_ptr<Block> block_ = nullptr;

	std::unique_ptr<Map> map_ = nullptr;


	std::unique_ptr<Player> player_ = nullptr;
	

	//ポーズシステム
	std::unique_ptr<PoseSystem> poseSystem_ = nullptr;

	static std::string pendingStageName_;  // StageSelect から受け取る一時引数
};

