#include "Player.h"
#include "WinApp.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <algorithm>

void Player::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//インプットの初期化
	input_ = Input::GetInstance();
	input_->SetIsMouseDisplay(false);
	input_->SetIsMouseFixed(true);
	//インスタンスの生成と初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, "player");
	object3d_->worldTransform.translate.y += 2.7f;

	//当たり判定の半径を設定
	radius_ = 2.5f;
	//当たり判定の属性を設定
	SetCollisionAttribute(CollisionAttribute::Player);

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/player");
	//パラメータのセット
	maxHP_ = param_["maxHP"];
	hp_ = maxHP_;

}

void Player::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//回転処理
	Rotate();
	//移動処理
	Move();
	//攻撃処理
	Attack();
	//弾の更新
	UpdateBullets();
	//死亡処理
	DeadProcess();

	//カメラ処理
	CameraAlgorithm();
}

void Player::Draw() {
	//オブジェクトの描画
	object3d_->Draw(camera_);
	//弾の描画
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}

}

void Player::DrawLine() {
	//ベースキャラクターのライン描画
	BaseCharacter::DrawLine();
	//弾のライン描画
	for (auto& bullet : bullets_) {
		bullet->DrawLine();
	}
}

void Player::DebugWithImGui() {
#ifdef _DEBUG
	ImGui::Begin("プレイヤー");
	ImGui::DragFloat3("平行移動", &object3d_->worldTransform.translate.x, 0.01f);
	ImGui::DragFloat3("回転", &object3d_->worldTransform.rotate.x, 0.01f);
	//HP
	ImGui::DragInt("HP", &hp_, 1, 0, maxHP_);

	ImGui::End();

	//弾のデバッグ
	for (auto& bullet : bullets_) {
		bullet->DebugWithImGui();
	}

	//当たり判定可視化用ラインの色を変更
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

	//F1キーでマウスカーソルの表示する
	if (input_->TriggerKey(DIK_F1)) {
		input_->SetIsMouseDisplay(true);
		input_->SetIsMouseFixed(false);
	}

#endif // _DEBUG
}

void Player::OnCollision(CollisionAttribute attribute) {
	//当たり判定時の処理
	switch (attribute) {
		//敵に当たった場合
	case CollisionAttribute::Enemy:
		break;
		//敵弾に当たった場合
	case CollisionAttribute::EnemyBullet:
		//HPを減らす
		hp_ -= 10;
		//0~MaxHPの範囲に収める
		hp_ = std::clamp(hp_, 0, maxHP_);
		//カメラシェイクを入れる
		camera_->RegistShake(0.4f, 0.5f);

		break;
	default:
		break;
	}
}

void Player::Rotate() {
	auto ShortestAngleDiff = [=](float from, float to) -> float {
		float diff = to - from;
		while (diff > pi)  diff -= 2 * pi;
		while (diff < -pi) diff += 2 * pi;
		return diff;
		};

	float cameraRotateY = camera_->worldTransform.rotate.y;
	float vehicleRotateY = object3d_->worldTransform.rotate.y;

	//最短角度差を求める
	float angleDiff = ShortestAngleDiff(vehicleRotateY, cameraRotateY);

	//回転速度の上限
	float rotateSpeed = param_["rotateSpeed"];
	float addRotation = rotateSpeed * kDeltaTime;

	//回転すべき角度が小さい場合は目標角度を代入
	if (std::abs(angleDiff) <= addRotation) {
		vehicleRotateY = cameraRotateY;
	}
	else {
		//回転方向に応じて加算または減算
		vehicleRotateY += (angleDiff > 0 ? 1 : -1) * addRotation;

		//-π ～ π に整える
		if (vehicleRotateY > pi)  vehicleRotateY -= 2 * pi;
		if (vehicleRotateY < -pi) vehicleRotateY += 2 * pi;
	}

	//オブジェクトの水平回転量に代入
	object3d_->worldTransform.rotate.y = vehicleRotateY;

}

void Player::Move() {
	//現在の向き(水平向きのみを考慮)
	Vector3 currentDir = {
		std::sinf(object3d_->worldTransform.rotate.y),
		0.0f,
		std::cosf(object3d_->worldTransform.rotate.y)
	};
	currentDir.Normalize();
	//WSキー入力で前後移動
	float speed = param_["speed"];
	if (input_->PushKey(DIK_W)) {
		velocity_ += currentDir * speed;
	}
	if (input_->PushKey(DIK_S)) {
		velocity_ += -currentDir * speed;
	}

	//床の抵抗値を加算
	Vector3 frictionDir = -velocity_.Normalized();
	Vector3 frictionAccel = frictionDir * floorFriction_;
	velocity_ += frictionAccel * kDeltaTime;

	//移動量の大きさを制限
	float maxSpeed = param_["maxSpeed"];
	if (velocity_.Length() > maxSpeed) {
		velocity_.Normalize();
		velocity_ *= maxSpeed;
	}
	//移動量の小ささを制限
	if (Vector3(velocity_ * kDeltaTime).Length() < 0.01f) {
		velocity_ = { 0.0f,0.0f,0.0f };
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
		float orx = camera_->worldTransform.rotate.x;
		float ory = camera_->worldTransform.rotate.y;
		Vector3 currentDir = {
			std::cosf(orx) * std::sinf(ory),
			-std::sinf(orx),		//←角度
			std::cosf(orx) * std::cosf(ory)
		};
		currentDir.Normalize();
		Vector3 bulletPos = object3d_->worldTransform.translate;
		bulletPos.y += 1.7f;
		Vector3 bulletDirection = currentDir;
		bullet->SetInitParam(bulletPos, bulletDirection);
		//リストに追加
		bullets_.push_back(std::move(bullet));
		//カメラシェイクを入れる
		camera_->RegistShake(0.2f, 0.15f);
	}

}

void Player::UpdateBullets() {
	//弾の削除
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		if ((*it)->GetIsDead()) {
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

void Player::DeadProcess() {
	//HPが0になったら死亡
	if (hp_ <= 0) {
		//死亡予約
		SetDeadTimer(0.1f);
	}
}

void Player::CameraAlgorithm() {
	//カメラの操作にオブジェクトの回転を合わせる
	Vector2 moveValue = input_->GetMousePosition();
	//デッドゾーン
	float deadZone = 3.0f;
	if (moveValue.Length() > deadZone) {
		camera_->worldTransform.rotate.x += moveValue.y * 0.0005f;
		camera_->worldTransform.rotate.y += moveValue.x * 0.0005f;
	}
	//回転制限
	const float maxPitch = (pi / 30.0f);		//下向き制限
	const float minPitch = -(pi / 15.0f);		//上向き制限
	camera_->worldTransform.rotate.x = std::clamp(camera_->worldTransform.rotate.x, minPitch, maxPitch);
	//水平回転をπ~-πの間に収める
	if (camera_->worldTransform.rotate.y > pi)
		camera_->worldTransform.rotate.y -= pi * 2.0f;
	else if (camera_->worldTransform.rotate.y < -pi)
		camera_->worldTransform.rotate.y += pi * 2.0f;

	//カメラの座標を決める
	camera_->worldTransform.translate = object3d_->worldTransform.translate;
	camera_->worldTransform.translate.y += 0.5f;
}
