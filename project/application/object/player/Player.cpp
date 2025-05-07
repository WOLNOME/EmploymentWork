#include "Player.h"
#include "ImGuiManager.h"

void Player::Initialize() {
	//インプットの初期化
	input_ = Input::GetInstance();
	//インスタンスの生成と初期化
	worldTransform_.Initialize();
	worldTransform_.translate_.y += 2.7f;
	object3d_ = std::make_unique<Object3d>();
	object3d_->InitializeModel("snowplow");

}

void Player::Update() {
	//移動処理
	Move();
	//カメラ処理
	CameraAlgorithm();

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Player::Draw(BaseCamera& _camera, SceneLight* _light) {
	//オブジェクトの描画
	object3d_->Draw(worldTransform_, _camera, _light);
}

void Player::DebugWithImGui() {
#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::DragFloat3("translate", &worldTransform_.translate_.x, 0.01f);
	ImGui::End();

#endif // _DEBUG
}

void Player::Move() {
	//現在の向き
	Vector3 currentDir = {
		std::sinf(worldTransform_.rotate_.y),
		0.0f,
		std::cosf(worldTransform_.rotate_.y)
	};
	currentDir.Normalize();
	//WSキー入力で前後移動
	if (input_->PushKey(DIK_W)) {
		velocity_ += currentDir * speed_;
	}
	if (input_->PushKey(DIK_S)) {
		velocity_ += -currentDir * speed_;
	}
	//ADキー入力で回転
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotate_.y += -rotateSpeed_ * kDeltaTime;
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.rotate_.y += rotateSpeed_ * kDeltaTime;
	}

	//床の抵抗値を加算
	Vector3 frictionDir = -velocity_.Normalized();
	Vector3 frictionAccel = frictionDir * floorRigist_;
	velocity_ += frictionAccel * kDeltaTime;

	//速度を加算
	worldTransform_.translate_ += velocity_ * kDeltaTime;

}

void Player::CameraAlgorithm() {
	//現在の向き
	Vector3 currentDir = Vector3{
		std::sinf(worldTransform_.rotate_.y),
		0.0f,
		std::cosf(worldTransform_.rotate_.y)
	}.Normalize();
	//カメラの座標を設定
	Vector3 cameraTranslate = {};
	cameraTranslate = worldTransform_.translate_ + (- currentDir) * 120.0f;
	cameraTranslate.y += 30.0f;
	camera_->SetTranslate(cameraTranslate);
	//カメラの回転を設定
	Vector3 direction = worldTransform_.translate_ - camera_->GetTranslate();
	direction.Normalize();
	float yaw = std::atan2f(direction.x, direction.z);
	float pitch = std::asinf(-direction.y);
	float roll = 0.0f;
	Vector3 euler = { pitch,yaw,roll };
	camera_->SetRotate(euler);

}
