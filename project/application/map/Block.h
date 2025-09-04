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



private:
	std::unique_ptr<Object3d> object3d_ = nullptr;

};

