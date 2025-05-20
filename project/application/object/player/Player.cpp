#include "Player.h"
#include "ImGuiManager.h"

void Player::Initialize() {
	//インプットの初期化
	input_ = Input::GetInstance();
	//インスタンスの生成と初期化
	
	object3d_->worldTransform.translate.y += 2.7f;
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, "snowplow");

}

void Player::Update() {
	//移動処理
	Move();
	//攻撃処理
	Attack();
	//弾の更新
	UpdateBullets();

	//カメラ処理
	CameraAlgorithm();

	//オブジェクトの更新
	object3d_->Update();
}

void Player::Draw() {
	//オブジェクトの描画
	object3d_->Draw(camera_);
	//弾の描画
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}

}

void Player::DebugWithImGui() {
#ifdef _DEBUG
	ImGui::Begin("player");
	ImGui::DragFloat3("translate", &object3d_->worldTransform.translate.x, 0.01f);
	ImGui::End();

	//弾のデバッグ
	for (auto& bullet : bullets_) {
		bullet->DebugWithImGui();
	}

#endif // _DEBUG
}

void Player::Move() {
	//現在の向き
	Vector3 currentDir = {
		std::sinf(object3d_->worldTransform.rotate.y),
		0.0f,
		std::cosf(object3d_->worldTransform.rotate.y)
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
		object3d_->worldTransform.rotate.y += -rotateSpeed_ * kDeltaTime;
	}
	if (input_->PushKey(DIK_D)) {
		object3d_->worldTransform.rotate.y += rotateSpeed_ * kDeltaTime;
	}

	//床の抵抗値を加算
	Vector3 frictionDir = -velocity_.Normalized();
	Vector3 frictionAccel = frictionDir * floorRegist_;
	velocity_ += frictionAccel * kDeltaTime;

	//移動量の大きさを制限
	if (velocity_.Length() > maxSpeed_) {
		velocity_.Normalize();
		velocity_ *= maxSpeed_;
	}

	//速度を加算
	object3d_->worldTransform.translate += velocity_ * kDeltaTime;

}

void Player::Attack() {
	//スペースキーで弾を発射
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾のインスタンスを生成
		std::unique_ptr<PlayerBullet> bullet = std::make_unique<PlayerBullet>();
		bullet->Initialize();
		//セット
		bullet->SetCamera(camera_);
		bullet->SetSceneLight(light_);
		//初期位置と初速度をセット
		Vector3 currentDir = {
			std::sinf(object3d_->worldTransform.rotate.y),
			0.5f,		//←角度
			std::cosf(object3d_->worldTransform.rotate.y)
		};
		currentDir.Normalize();
		Vector3 bulletVelocity = currentDir * 100.0f;
		bullet->SetInitParam(object3d_->worldTransform.translate, bulletVelocity);
		//リストに追加
		bullets_.push_back(std::move(bullet));
	}

}

void Player::UpdateBullets() {
	//弾の削除
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		if ((*it)->GetWorldTransform().translate.y < -10.0f) {
			it = bullets_.erase(it);
		}
		else {
			++it;
		}
	}
	//弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
	}
}

void Player::CameraAlgorithm() {
	//現在の向き
	Vector3 currentDir = Vector3{
		std::sinf(object3d_->worldTransform.rotate.y),
		0.0f,
		std::cosf(object3d_->worldTransform.rotate.y)
	}.Normalize();
	//カメラの座標を設定
	Vector3 cameraTranslate = {};
	cameraTranslate = object3d_->worldTransform.translate + (-currentDir) * 120.0f;
	cameraTranslate.y += 30.0f;
	camera_->SetTranslate(cameraTranslate);
	//カメラの回転を設定
	Vector3 direction = object3d_->worldTransform.translate - camera_->GetTranslate();
	direction.Normalize();
	float yaw = std::atan2f(direction.x, direction.z);
	float pitch = std::asinf(-direction.y);
	float roll = 0.0f;
	Vector3 euler = { pitch,yaw,roll };
	camera_->SetRotate(euler);

}
