#include "Enemy.h"
#include "ImGuiManager.h"

void Enemy::Initialize() {
	//インスタンスの生成と初期化
	worldTransform_.Initialize();
	worldTransform_.translate_.z += 200.0f;
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
	//もしプレイヤーが索敵範囲内にいないなら処理を行わない
	if (player_->GetWorldTransform().translate_.Distance(worldTransform_.translate_) > searchPlayerDistance_) {
		return;
	}
	//プレイヤーへの方向を求める
	Vector3 dirToPlayer = player_->GetWorldTransform().translate_ - worldTransform_.translate_;
	//y座標は考慮しない
	dirToPlayer.y = 0.0f;
	//正規化
	dirToPlayer.Normalize();

	//移動量を求める
	velocity_ += dirToPlayer * speed_;
	//摩擦力をかける
	Vector3 frictionDir = -velocity_.Normalized();
	Vector3 frictionAccel = frictionDir * floorRegist_;
	velocity_ += frictionAccel * kDeltaTime;

	//移動量の大きさを制限
	if (velocity_.Length() > maxSpeed_) {
		velocity_.Normalize();
		velocity_ *= maxSpeed_;
	}

	//速度を加算
	worldTransform_.translate_ += velocity_ * kDeltaTime;

	//移動方向に向かって回転->現在の向きを求める
	Vector3 currentDir = {
		std::sinf(worldTransform_.rotate_.y),
		0.0f,
		std::cosf(worldTransform_.rotate_.y)
	};
	currentDir.Normalize();
	//目標の向きを求める
	Vector3 targetDir = dirToPlayer;
	targetDir.Normalize();
	//回転の差を求める(float型)
	float angle = std::atan2f(targetDir.x, targetDir.z) - std::atan2f(currentDir.x, currentDir.z);
	//もしこの角度がPIより大きい場合は2PI引き、PIより小さい場合は2PI足す
	if (angle > pi) {
		angle -= 2 * pi;
	}
	else if (angle < -pi) {
		angle += 2 * pi;
	}
	//もし、この角度の絶対値が回転スピードより小さい場合は、この角度をそのまま回転スピードとする
	float usingRotateSpeed;
	if (std::abs(angle) < rotateSpeed_ * kDeltaTime) {
		usingRotateSpeed = angle;
	}
	else {
		//回転スピードを使う場合、符号を揃える
		usingRotateSpeed = (angle > 0) ? rotateSpeed_ * kDeltaTime : -rotateSpeed_ * kDeltaTime;
	}
	//ワールドトランスフォームの回転を加算
	worldTransform_.rotate_.y += usingRotateSpeed;
	//回転の範囲を-PI~PIに収める
	if (worldTransform_.rotate_.y > pi) {
		worldTransform_.rotate_.y -= 2 * pi;
	}
	else if (worldTransform_.rotate_.y < -pi) {
		worldTransform_.rotate_.y += 2 * pi;
	}


}
