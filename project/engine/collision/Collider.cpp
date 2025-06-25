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
			.min = collisionLocalAABB_.min + GetWorldPosition(),
			.max = collisionLocalAABB_.max + GetWorldPosition()
		};
		MyMath::CreateLineAABB(aabb, debugLineColor_);
		break;
	}
	case Collider::CollisionShapeKind::OBB: {
		break;
	}
	default:
		break;
	}
#endif //_DEBUG
}
