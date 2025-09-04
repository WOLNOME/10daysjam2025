#include "Block.h"
#include "engine/3d/Object3dManager.h"

Block::Block() {
}

Block::~Block() {
}

void Block::Initialize(MapChipType mapChipType, Vector3 translate)
{

	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Block"), ToFile(mapChipType));
	object3d_->worldTransform.scale = { 1.0f,1.0f,1.0f };
	object3d_->worldTransform.translate = { translate };


}

void Block::Update() {

}

void Block::Draw() {

}