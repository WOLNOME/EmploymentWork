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
	Sphere sphere = Sphere(
		GetWorldPosition(),
		radius_
	);
	MyMath::CreateLineSphere(sphere, { 1,1,1,1 }, lineDrawer_.get());
	//描画
	lineDrawer_->Draw(*_camera);

#endif // _DEBUG
}
