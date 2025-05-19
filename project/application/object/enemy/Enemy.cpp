#include "Enemy.h"
#include "ImGuiManager.h"

void Enemy::Initialize() {
	//インスタンスの生成と初期化
	worldTransform_.Initialize();
	worldTransform_.translate_.z += 20.0f;
	worldTransform_.translate_.y += 2.7f;
	object3d_ = std::make_unique<Object3d>();
	object3d_->InitializeModel("snowplow");

}

void Enemy::Update() {
	//移動処理
	Move();

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(SceneLight* _light) {
	//オブジェクトの描画
	object3d_->Draw(worldTransform_, *camera_, _light);
}

void Enemy::DebugWithImGui() {
#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::DragFloat3("translate", &worldTransform_.translate_.x, 0.01f);
	ImGui::End();

#endif // _DEBUG
}

void Enemy::Move() {
	//現在の向き
	Vector3 currentDir = {
		std::sinf(worldTransform_.rotate_.y),
		0.0f,
		std::cosf(worldTransform_.rotate_.y)
	};
	currentDir.Normalize();
	

	//床の抵抗値を加算
	Vector3 frictionDir = -velocity_.Normalized();
	Vector3 frictionAccel = frictionDir * floorRigist_;
	velocity_ += frictionAccel * kDeltaTime;

	//速度を加算
	worldTransform_.translate_ += velocity_ * kDeltaTime;

}
