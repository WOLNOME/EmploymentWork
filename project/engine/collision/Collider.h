#pragma once
#include "Vector3.h"
#include "CollisionConfig.h"
class Collider {

public:
	virtual ~Collider() = default;
	float GetRadius() { return radius_; }
	virtual void OnCollision() = 0;
	virtual Vector3 GetWorldPosition() = 0;
	CollisionAttribute GetCollisionAttribute() { return collisionAttribute_; }
	void SetCollisionAttribute(const CollisionAttribute collisionAttribute) { collisionAttribute_ = collisionAttribute; }

private:
	CollisionAttribute collisionAttribute_ = CollisionAttribute::Nothingness;

protected:
	float radius_;
};
