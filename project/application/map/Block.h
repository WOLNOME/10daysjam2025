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


private:
	std::unique_ptr<Object3d> object3d_ = nullptr;

	Vector3 scale_ = { 1.0f,1.0f,1.0f };

};

