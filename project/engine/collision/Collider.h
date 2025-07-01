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
	virtual void OnCollision(CollisionAttribute attribute) = 0;

	//デバッグ専用処理
	virtual void DebugWithImGui();

public://getter
	//形状の種類を取得
	CollisionShapeKind GetCollisionShapeKind() const { return collisionShapeKind_; }
	//球体半径情報を取得
	float GetRadius() { return collisionRadius_; }
	//AABBローカル情報を取得
	const AABB& GetAABB() { return collisionLocalAABB_; }
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
	AABB collisionLocalAABB_ = {
		.min = { -1.0f, -1.0f, -1.0f },	//最小座標
		.max = { 1.0f, 1.0f, 1.0f }		//最大座標
	};

	//OBB用変数
	OBB collisionLocalOBB_ = {
		.center = {0.0f,0.0f,0.0f},
		.orientations = {{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}},
		.size = {1.0f,1.0f,1.0f}
	};

	//デバッグ用変数
	Vector4 debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };
};
