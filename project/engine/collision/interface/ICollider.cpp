#include "ICollider.h"

using namespace Norm;

const Vector3& ICollider::GetCenter() {
	//オフセットをワールド空間に（平行移動・スケールは考慮しない）
	Matrix4x4 rotateMat = MyMath::MakeRotateMatrix(worldTransform_->GetRotate());
	Vector3 worldOffset = MyMath::TransformNormal(offset_, rotateMat);
	
	return worldTransform_->GetTranslate() + worldOffset;
}