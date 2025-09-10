#include "BackPlane.h"
#include <Object3dManager.h>
#include <imgui.h>

void BackPlane::Initialize() {
	floor_ = std::make_unique<Object3d>();
	floor_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Floor"),"floor");

	floor_->worldTransform.scale = { 100.0f,1.0f,100.0f };
	floor_->worldTransform.translate = { 0.0f,-0.5f,0.0f };
	
}

void BackPlane::Update() {
	ImGui::Begin("BackPlane");
	ImGui::DragFloat3("Translate", &floor_->worldTransform.translate.x, 0.1f);
	ImGui::DragFloat3("Rotate", &floor_->worldTransform.rotate.x, 0.1f);
	ImGui::DragFloat3("Scale", &floor_->worldTransform.scale.x, 0.1f);
	ImGui::End();

}