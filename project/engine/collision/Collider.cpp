#include "Collider.h"
#include "MyMath.h"

void Collider::DebugWithImGui() {
#ifdef _DEBUG
	//形状の種類によって分岐
	switch (collisionShapeKind_) {
	case Collider::CollisionShapeKind::Sphere: {
		//球体を定義
		Sphere sphere = {
			.center = GetWorldPosition(),
			.radius = collisionRadius_
		};
		MyMath::CreateLineSphere(sphere, debugLineColor_);
		break;
	}
	case Collider::CollisionShapeKind::AABB: {
		//AABBを定義
		AABB aabb = {
			.min = collisionMinAABB_ + GetWorldPosition(),
			.max = collisionMaxAABB_ + GetWorldPosition()
		};
		MyMath::CreateLineAABB(aabb, debugLineColor_);
		break;
	}
	case Collider::CollisionShapeKind::OBB: {
		//回転行列
		Matrix4x4 matRotate = MyMath::MakeRotateMatrix(GetRotate());
		//OBBを定義
		OBB obb = {
			.center = GetWorldPosition() + collisionCenterOffsetOBB_,
			.orientations = {MyMath::TransformNormal(Vector3(1,0,0),matRotate),MyMath::TransformNormal(Vector3(0,1,0),matRotate),MyMath::TransformNormal(Vector3(0,0,1),matRotate)},
			.size = collisionSizeOBB_
		};
		//線を登録
		MyMath::CreateLineOBB(obb, debugLineColor_);

		break;
	}
	default:
		break;
	}
#endif //_DEBUG
}
