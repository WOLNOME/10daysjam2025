#pragma once
#include "engine/3d/Object3d.h"
#include <memory>
#include "MapChipType.h"
class Block
{

public:
	Block();
	~Block();
	// 初期化
	void Initialize(MapChipType mapChipType,Vector3 translate);
	// 更新
	void Update();
	// 描画
	void Draw();


	float GetScaleY() const { return scale_.y; }

	// ワールド座標を直接セット（押し移動で使用）
	void SetWorldPosition(const Vector3& p) {
		if (object3d_) { object3d_->worldTransform.translate = p; }
	}

	// オブジェクトを空にする(RedoUndoで使用)
	void ClearObject() {
		object3d_.reset();
	}


private:
	std::unique_ptr<Object3d> object3d_ = nullptr;

	Vector3 scale_ = { 1.0f,1.0f,1.0f };

};

