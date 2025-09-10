#pragma once
#include "engine/3d/Object3d.h"
class BackPlane
{
public:
	void Initialize();

	void Update();

private:
	std::unique_ptr<Object3d> floor_ = nullptr;
};

