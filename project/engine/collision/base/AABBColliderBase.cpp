#include "AABBColliderBase.h"

Norm::AABBColliderBase::AABBColliderBase() {
	//コライダーの形状を設定
	colliderShape_ = ColliderShape::AABB;
}
