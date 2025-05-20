#include "PlayerBullet.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

void PlayerBullet::Initialize() {
	//インスタンスの生成と初期化
	TextureManager::GetInstance()->LoadTexture("black.png");
	object3d_->worldTransform.Initialize();
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{}, Shape::kSphere);
}

void PlayerBullet::Update() {
	//移動処理
	Move();

	//ワールドトランスフォームの更新
	object3d_->worldTransform.UpdateMatrix();
}

void PlayerBullet::Draw() {
	//オブジェクトの描画
	object3d_->Draw(camera_,textureHandle_);
}

void PlayerBullet::DebugWithImGui() {
#ifdef _DEBUG

	ImGui::Begin("playerBullet");
	ImGui::DragFloat3("translate", &object3d_->worldTransform.translate.x, 0.01f);
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
	object3d_->worldTransform.translate += velocity_ * kDeltaTime;
}
