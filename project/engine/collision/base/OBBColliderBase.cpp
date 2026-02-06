#include "OBBColliderBase.h"
#include <MyMath.h>

using namespace Norm;

Norm::OBBColliderBase::OBBColliderBase() {
	//コライダーの形状を設定
	colliderShape_ = ColliderShape::OBB;
}

const OBB& OBBColliderBase::GetOBB() {
	//回転行列
	Matrix4x4 matRotate = MyMath::MakeRotateMatrix(worldTransform_->GetRotate());

	//OBBを作成
	OBB result;
	result.center = GetCenter();
	result.orientations[0] = MyMath::TransformNormal(Vector3(1, 0, 0), matRotate);
	result.orientations[1] = MyMath::TransformNormal(Vector3(0, 1, 0), matRotate);
	result.orientations[2] = MyMath::TransformNormal(Vector3(0, 0, 1), matRotate);
	result.size = size_;

	return result;
}
