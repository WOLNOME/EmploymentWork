#include "SphereColliderBase.h"

using namespace Norm;

Norm::SphereColliderBase::SphereColliderBase() {
	//コライダーの形状を設定
	colliderShape_ = ColliderShape::Sphere;
}

const Capsule& SphereColliderBase::GetCapsule() {
	//カプセルを作成
	Capsule result;
	result.radius = radius_;
	result.segment.origin = GetPreWorldTranslate();
	result.segment.diff =
		worldTransform_->GetWorldTranslate()
		- GetPreWorldTranslate();

	return result;
}
