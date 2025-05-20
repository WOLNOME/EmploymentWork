#include "PlayerBullet.h"
#include "TextureManager.h"
#include "ImGuiManager.h"

void PlayerBullet::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//インスタンスの生成と初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("black.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{}, Shape::kSphere);

	//当たり判定の半径を設定
	radius_ = 1.5f;

}

void PlayerBullet::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//移動処理
	Move();
}

void PlayerBullet::Draw() {
	//オブジェクトの描画
	object3d_->Draw(camera_, textureHandle_);
}

void PlayerBullet::DrawLine() {
	//ベースキャラクターのライン描画
	BaseCharacter::DrawLine();
}

void PlayerBullet::DebugWithImGui() {
#ifdef _DEBUG

	ImGui::Begin("playerBullet");
	ImGui::DragFloat3("translate", &object3d_->worldTransform.translate.x, 0.01f);
	ImGui::End();

#endif // _DEBUG

}

void PlayerBullet::OnCollision(CollisionAttribute attribute) {
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
