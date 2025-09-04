#include "Monkey.h"

void Monkey::Initialize()
{
	//3Dオブジェクトの生成と初期化
	character_ = std::make_unique<Object3d>();
	character_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Monkey"), "monkey");

	character_->worldTransform.translate = { 0.0f,5.0f,0.0f };
	
}

void Monkey::Update()
{
	character_->worldTransform.rotate.y -= 0.03f;
}
