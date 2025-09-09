#pragma once
#include "Block.h"
#include "CsvLoader.h"
#include "Vector3.h"
#include <optional>
#include "application/map/MapTypes.h"
#include "application/system/RedoUndoSystem.h"


class Map {

public:
	// 初期化
	void Initialize(const std::string& filepath);

	// 犬/猿のスポーン位置（ワールド座標）を返す
	std::optional<Vector3> GetDogSpawnWorld() const;
	std::optional<Vector3> GetMonkeySpawnWorld() const;
	// スポーン（グリッド/ワールド）取得
	std::optional<GridPos> GetDogSpawnGrid() const;
	std::optional<GridPos> GetMonkeySpawnGrid() const;

	// 歩行可能判定（タイルで決める）
	bool IsWalkableFor(ActorKind who, int gx, int gy) const;

	// グリッド→ワールド（犬/猿の高さ込み）
	Vector3 WorldFromGridFor(ActorKind who, int gx, int gy) const;

	// 犬が箱を押す（成功なら true）
	bool TryPushBlockByDog(int dogGx, int dogGy, int dx, int dy, const GridPos& monkeyPos);

	// (gx,gy) の L2 に BlockMonkey があるか？
	bool HasBlockMonkeyAt(int gx, int gy) const;

	// RedoUndoシステムのgetter
	RedoUndoSystem* GetRedoUndoSystem() const {
		return redoUndoSystem_.get();
	}
	// ゴール判定（who のレイヤで (gx,gy) がゴールなら true）
	bool IsGoalFor(ActorKind who, int gx, int gy) const;


	StageCamera GetStageCameraVal() { return stageCamera_; }

	//モデルのあるタイルかどうか(PlayerでもRedoUndo処理で使うためpublic関数にした)
	static bool IsRenderable(MapChipType t);
	// プレイヤーが (gx,gy) に入ったときのトグル処理（踏んだ瞬間だけ呼ぶ）
	void OnPlayerStepped(ActorKind who, int gx, int gy);

	void SetAllBootBlocks(bool toOn);
	void SetBootBlockAt(int gx, int gy, bool toOn);


	// layer2 上の BlockMonkey の配置 -> Block* を保持
	// サイズは [height][width]。BlockMonkey があるセルに対応する Block* を格納（なければ nullptr）
	std::vector<std::vector<Block*>> l2BlockAt_;

	// 一層
	std::vector<std::unique_ptr<Block>> blocksL1_;
	// 二層
	std::vector<std::unique_ptr<Block>> blocksL2_;
	// 床ブロックの高さ（scale.y）
	float   blockScaleY_ = 1.0f;
	CsvMapData csvMapData_;

private:

	//RedoUndoシステム
	std::unique_ptr<RedoUndoSystem> redoUndoSystem_ = nullptr;

	// BootBlock のブロック参照テーブル（描画オブジェクトのYを切替用）
	std::vector<std::vector<Block*>> l1BootAt_;
	std::vector<std::vector<Block*>> l2BootAt_;

	bool blockScaleCaptured_ = false;
;
	// タイル→ワールド変換
	float   tileSize_ = 2.0f;
	Vector3 origin_ = { 0,0,0 };
	float   overlayYOffset_ = 0.01f;

	// グリッド→ワールド座標変換
	Vector3 GridToWorld(int gx, int gy, float yOffset) const;
	std::optional<Vector3> FindStartOnLayer(
		const std::vector<std::vector<MapChipType>>& layer,
		MapChipType target, float yOffset) const;

	// ステージカメラ
	StageCamera stageCamera_;
	int cameraType_ = 0; 
};

