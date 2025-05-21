#include "Enemy.h"
#include "ImGuiManager.h"

void Enemy::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();
	//インスタンスの生成と初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, "snowplow");
	if (light_) {
		object3d_->SetSceneLight(light_);
	}
	object3d_->worldTransform.translate.z += 200.0f;
	object3d_->worldTransform.translate.y += 2.7f;

	//当たり判定の半径を設定
	radius_ = 3.8f;
	//当たり判定の属性を設定
	SetCollisionAttribute(CollisionAttribute::Enemy);
	

}

void Enemy::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//移動処理
	Move();

	//オブジェクトの更新
	object3d_->Update();
}

void Enemy::Draw() {
	//オブジェクトの描画
	object3d_->Draw(camera_);
}

void Enemy::DrawLine() {
	//ベースキャラクターのライン描画
	BaseCharacter::DrawLine();
}

void Enemy::DebugWithImGui() {
#ifdef _DEBUG
	ImGui::Begin("enemy");
	ImGui::DragFloat3("translate", &object3d_->worldTransform.translate.x, 0.01f);
	ImGui::End();

	//デバッグ用ラインのカラー
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

#endif // _DEBUG
}

void Enemy::OnCollision(CollisionAttribute attribute) {
	//当たり判定時の処理
	switch (attribute) {
	case CollisionAttribute::Player:
		//プレイヤーに当たった場合
		break;
	case CollisionAttribute::PlayerBullet:
		//プレイヤー弾に当たった場合
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		break;
	default:
		break;
	}
}

void Enemy::Move() {
	//もしプレイヤーが索敵範囲内にいないなら処理を行わない
	if (player_->GetWorldTransform().translate.Distance(object3d_->worldTransform.translate) > searchPlayerDistance_) {
		return;
	}
	//プレイヤーへの方向を求める
	Vector3 dirToPlayer = player_->GetWorldTransform().translate - object3d_->worldTransform.translate;
	//y座標は考慮しない
	dirToPlayer.y = 0.0f;
	//正規化
	dirToPlayer.Normalize();

	//移動量を求める
	velocity_ += dirToPlayer * speed_;
	//摩擦力をかける
	Vector3 frictionDir = -velocity_.Normalized();
	Vector3 frictionAccel = frictionDir * floorFriction_;
	velocity_ += frictionAccel * kDeltaTime;

	//移動量の大きさを制限
	if (velocity_.Length() > maxSpeed_) {
		velocity_.Normalize();
		velocity_ *= maxSpeed_;
	}

	//速度を加算
	object3d_->worldTransform.translate += velocity_ * kDeltaTime;

	//移動方向に向かって回転->現在の向きを求める
	Vector3 currentDir = {
		std::sinf(object3d_->worldTransform.rotate.y),
		0.0f,
		std::cosf(object3d_->worldTransform.rotate.y)
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
	object3d_->worldTransform.rotate.y += usingRotateSpeed;
	//回転の範囲を-PI~PIに収める
	if (object3d_->worldTransform.rotate.y > pi) {
		object3d_->worldTransform.rotate.y -= 2 * pi;
	}
	else if (object3d_->worldTransform.rotate.y < -pi) {
		object3d_->worldTransform.rotate.y += 2 * pi;
	}


}
