#pragma once
#include <Object3d.h>
#include <Object3dManager.h>
#include <TextureManager.h>
#include <Collider.h>

class CharacterBase
{
public:
	virtual ~CharacterBase() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();


	void SetWorldPosition(const Vector3& p) {
		if (character_) { character_->worldTransform.translate = p; }
	}

	const Vector3& GetWorldPosition() const { return character_->worldTransform.translate; }

	// ゴール時の演出：Y回転を加える
	void AddYaw(float delta) {
		if (character_) { character_->worldTransform.rotate.y += delta; }
	}

	float GetYaw() const { return character_->worldTransform.rotate.y; }
	void  SetYaw(float yaw) {
		character_->worldTransform.rotate.y = yaw;
		character_->worldTransform.UpdateMatrix();   // 行列の更新が必要な場合
	}

protected:

	//3Dオブジェクト
	std::unique_ptr<Object3d> character_ = nullptr;


private:


};

