#include "PlayerBullet.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

void PlayerBullet::Initialize() {
	//インスタンスの生成と初期化
	TextureManager::GetInstance()->LoadTexture("black.png");
	worldTransform_.Initialize();
	object3d_ = std::make_unique<Object3d>();
	object3d_->InitializeShape(Shape::kSphere);
}

void PlayerBullet::Update() {
	//移動処理
	Move();

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void PlayerBullet::Draw(BaseCamera* _camera, SceneLight* _light) {
	//オブジェクトの描画
	object3d_->Draw(worldTransform_, *_camera, _light, textureHandle_);
}

void PlayerBullet::DebugWithImGui() {
#ifdef _DEBUG

	ImGui::Begin("playerBullet");
	ImGui::DragFloat3("translate", &worldTransform_.translate_.x, 0.01f);
	ImGui::End();

#endif // _DEBUG

}

void PlayerBullet::Move() {
	//重力をかける
	velocity_.y -= gravity_ * kDeltaTime;
	//空気抵抗をかける
	Vector3 airResistanceDir = -velocity_.Normalized();
	Vector3 airResistanceAccel = airResistanceDir * airResistance_ * velocity_.Length();
	velocity_ += airResistanceAccel * kDeltaTime;
	//移動量の大きさを制限
	if (velocity_.Length() > maxSpeed_) {
		velocity_.Normalize();
		velocity_ *= maxSpeed_;
	}
	worldTransform_.translate_ += velocity_ * kDeltaTime;
}
