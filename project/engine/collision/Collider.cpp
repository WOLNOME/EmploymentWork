#include "Collider.h"
#include "MyMath.h"

void Collider::InitDebugLine() {
#ifdef _DEBUG
	lineDrawer_ = std::make_unique<LineDrawer>();
	lineDrawer_->Initialize();
#endif // _DEBUG
}

void Collider::DrawCollisionLine(BaseCamera* _camera) {
#ifdef _DEBUG
	lineDrawer_->ClearLine();
	//形状の種類によって分岐
	switch (collisionShapeKind_) {
	case Collider::CollisionShapeKind::Sphere: {
		//球体を定義
		Sphere sphere = {
			.center = GetWorldPosition(),
			.radius = collisionRadius_
		};
		MyMath::CreateLineSphere(sphere, debugLineColor_, lineDrawer_.get());
		break;
	}
	case Collider::CollisionShapeKind::AABB: {
		//AABBを定義
		AABB aabb = {
			.min = collisionLocalAABB_.min + GetWorldPosition(),
			.max = collisionLocalAABB_.max + GetWorldPosition()
		};
		MyMath::CreateLineAABB(aabb, debugLineColor_, lineDrawer_.get());
		break;
	}
	case Collider::CollisionShapeKind::OBB: {
		break;
	}
	default:
		break;
	}

	//描画
	lineDrawer_->Draw(*_camera);

#endif // _DEBUG
}
