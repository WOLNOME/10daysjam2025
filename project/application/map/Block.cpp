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
	object3d_->worldTransform.scale = { scale_ };
	object3d_->worldTransform.translate = { translate };

    object3d_->worldTransform.UpdateMatrix();

}

void Block::Update() {

}

void Block::Draw() {

}

void Block::ReplaceVisual(MapChipType newType)
{
    if (!object3d_) return;

    // いまのTransformを退避
    const auto& wt = object3d_->worldTransform;

    // 新しいObject3dを生成して同じTransformを適用
    auto newObj = std::make_unique<Object3d>();
    newObj->Initialize(
        ModelTag{},
        Object3dManager::GetInstance()->GenerateName("Block"),
        ToFile(newType)              // ← ファイル名をタイプから解決
    );
    newObj->worldTransform.scale = wt.scale;
    newObj->worldTransform.rotate = wt.rotate;
    newObj->worldTransform.translate = wt.translate;
    // 必要なら: 
    newObj->worldTransform.UpdateMatrix();

    // 置き換え
    object3d_ = std::move(newObj);
}
