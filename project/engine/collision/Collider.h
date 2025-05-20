#pragma once
#include "Vector3.h"
#include "CollisionConfig.h"
class Collider {

public:
	//デストラクタ
	virtual ~Collider() = default;
	//当たり判定時の処理
	virtual void OnCollision() = 0;

public://getter
	//半径を取得
	float GetRadius() { return radius_; }
	//ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;
	//属性の取得
	CollisionAttribute GetCollisionAttribute() { return collisionAttribute_; }

public://setter
	//属性のセット
	void SetCollisionAttribute(const CollisionAttribute collisionAttribute) { collisionAttribute_ = collisionAttribute; }

private:
	CollisionAttribute collisionAttribute_ = CollisionAttribute::Nothingness;

protected:
	float radius_;
};
