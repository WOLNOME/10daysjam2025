#pragma once
#include "BaseCamera.h"
#include "MyMath.h"
#include "Vector3.h"
#include "Vector4.h"
#include "CollisionConfig.h"
#include "WorldTransform.h"

class Collider {
public:
	//コリジョン形状の種類
	enum class CollisionShapeKind {
		Sphere,		//球体
		AABB,		//AABB
		OBB,		//OBB
	};

public:
	//デストラクタ
	virtual ~Collider() = default;

	//当たり判定時の処理
	virtual void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) = 0;

	//デバッグ専用処理
	virtual void DebugWithImGui();

public://getter
	//形状の種類を取得
	CollisionShapeKind GetCollisionShapeKind() const { return collisionShapeKind_; }
	//球体半径情報を取得
	float GetRadius() { return collisionRadius_; }
	//AABBローカル情報を取得
	const Vector3& GetMinAABB() { return collisionMinAABB_; }
	const Vector3& GetMaxAABB() { return collisionMaxAABB_; }
	//OBBローカル情報を取得
	const Vector3& GetCenterOffsetOBB() { return collisionCenterOffsetOBB_; }
	const Vector3& GetSizeOBB() { return collisionSizeOBB_; }
	//ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;
	//回転を取得
	virtual Vector3 GetRotate() = 0;
	//スケールを取得
	virtual Vector3 GetScale() = 0;
	//前フレーム座標を取得
	virtual Vector3 GetPreWorldPosition() = 0;
	//属性の取得
	CollisionAttribute GetCollisionAttribute() { return collisionAttribute_; }

public://setter
	//属性のセット
	void SetCollisionAttribute(const CollisionAttribute collisionAttribute) { collisionAttribute_ = collisionAttribute; }

private:
	CollisionAttribute collisionAttribute_ = CollisionAttribute::Nothingness;
protected:
	//形状の種類
	CollisionShapeKind collisionShapeKind_ = CollisionShapeKind::Sphere;

	//球体用変数
	float collisionRadius_ = 1.0f;	//半径

	//AABB用変数
	Vector3 collisionMinAABB_ = { -1,-1,-1 };
	Vector3 collisionMaxAABB_ = { 1,1,1 };

	//OBB用変数
	Vector3 collisionCenterOffsetOBB_ = { 0,0,0 };
	Vector3 collisionSizeOBB_ = { 1,1,1 };

	//デバッグ用変数
	Vector4 debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };
};
