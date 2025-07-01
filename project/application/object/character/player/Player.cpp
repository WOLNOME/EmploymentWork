#include "Player.h"
#include "WinApp.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "Object3dManager.h"
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
	object3d_->Initialize(ModelTag{},Object3dManager::GetInstance()->GenerateName("Player"), "player");
	object3d_->worldTransform.translate.y += 2.7f;

	//当たり判定の形状を設定
	collisionShapeKind_ = Collider::CollisionShapeKind::AABB;
	//当たり判定の大きさを設定
	collisionLocalAABB_ = {
		.min = { -4.0f, -2.3f, -4.0f },	//最小座標
		.max = { 4.0f, 1.7f, 4.0f }		//最大座標
	};
	//当たり判定の属性を設定
	SetCollisionAttribute(CollisionAttribute::Player);

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/player");
	//パラメータのセット
	maxHP_ = param_["maxHP"];
	hp_ = maxHP_;
	cannonReloadTime_ = param_["cannonReloadTime"];
	cannonReloadTimer_ = 0.0f;
	bulletReloadTime_ = param_["bulletReloadTime"];
	bulletReloadTimer_ = 0.0f;
	bulletMaxNum_ = param_["bulletMagazine"];
	bulletNum_ = bulletMaxNum_;
	bulletFireIntervalTime_ = param_["bulletFireIntervalTime"];
	bulletFireIntervalTimer_ = 0.0f;

}

void Player::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();
	//ダメージ更新
	isDamage_ = false;

	//F1キーでマウスカーソルの表示する
	if (input_->TriggerKey(DIK_F1)) {
		if (isDebug_) {
			//デバッグモードを終了
			isDebug_ = false;
			input_->SetIsMouseDisplay(false);
			input_->SetIsMouseFixed(true);
		}
		else {
			//デバッグモードを開始
			isDebug_ = true;
			input_->SetIsMouseDisplay(true);
			input_->SetIsMouseFixed(false);
		}
	}

	//回転処理
	Rotate();
	//移動処理
	Move();
	//攻撃処理
	CannonAttack();
	BulletAttack();
	//死亡処理
	DeadProcess();

	//カメラ処理
	CameraAlgorithm();
}

void Player::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

	ImGui::Begin("プレイヤー");
	ImGui::DragFloat3("平行移動", &object3d_->worldTransform.translate.x, 0.01f);
	ImGui::DragFloat3("回転", &object3d_->worldTransform.rotate.x, 0.01f);
	//HP
	ImGui::DragInt("HP", &hp_, 1, 0, maxHP_);
	//機関銃
	ImGui::Text("機関銃");
	ImGui::Text("リロードタイム: %.2f", bulletReloadTimer_);
	ImGui::Text("発射間隔タイマー: %.2f", bulletFireIntervalTimer_);
	ImGui::Text("現在の弾数: %d", bulletNum_);

	ImGui::End();

	//当たり判定可視化用ラインの色を変更
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

#endif // _DEBUG
	
}

void Player::OnCollision(CollisionAttribute attribute) {
	//当たり判定時の処理
	switch (attribute) {
		//敵に当たった場合
	case CollisionAttribute::Enemy:
		break;
		//敵キャノンに当たった場合
	case CollisionAttribute::EnemyCannon:
		//HPを減らす
		hp_ -= 10;
		//0~MaxHPの範囲に収める
		hp_ = std::clamp(hp_, 0, maxHP_);
		//カメラシェイクを入れる
		camera_->RegistShake(0.4f, 0.8f);

		//ダメージヒット
		isDamage_ = true;

		break;
		//敵弾に当たった場合
	case CollisionAttribute::EnemyBullet:
		//HPを減らす
		hp_ -= 1;
		//0~MaxHPの範囲に収める
		hp_ = std::clamp(hp_, 0, maxHP_);
		//カメラシェイクを入れる
		camera_->RegistShake(0.2f, 0.3f);

		//ダメージヒット
		isDamage_ = true;

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

void Player::CannonAttack() {
	//リロードタイムの計算
	if (cannonReloadTimer_ > 0.0f) {
		cannonReloadTimer_ -= kDeltaTime;
		//リロードタイムがマイナスになったら0にする
		if (cannonReloadTimer_ < 0.0f) {
			cannonReloadTimer_ = 0.0f;
		}
		//砲弾を発射したフラグをオフ
		isCannonFire_ = false;
		//計算後はこの関数を抜ける
		return;
	}

	//スペースキーで砲弾を発射
	if (input_->TriggerKey(DIK_SPACE)) {
		//砲弾を発射したフラグをオン
		isCannonFire_ = true;
		//リロードタイムをセット
		cannonReloadTimer_ = cannonReloadTime_;
	}
}

void Player::BulletAttack() {
	//発射間隔の計算
	bool isInterval = false;
	if (bulletFireIntervalTimer_ > 0.0f) {
		isInterval = true;
		bulletFireIntervalTimer_ -= kDeltaTime;
		//発射間隔がマイナスになったら0にする
		if (bulletFireIntervalTimer_ < 0.0f) {
			bulletFireIntervalTimer_ = 0.0f;
		}
	}

	//リロードタイムの計算
	bool isReload = false;
	if (bulletReloadTimer_ > 0.0f) {
		isReload = true;
		bulletReloadTimer_ -= kDeltaTime;
		//リロードタイムがマイナスになったら0にする
		if (bulletReloadTimer_ < 0.0f) {
			bulletReloadTimer_ = 0.0f;
			//銃弾数をリロード
			bulletNum_ = bulletMaxNum_;
		}
	}

	//インターバルおよびリロード中は発射しない
	if (isInterval || isReload) {
		//銃弾を発射したフラグをオフ
		isBulletFire_ = false;
		//計算後はこの関数を抜ける
		return;
	}

	//左クリックで銃弾を発射
	if (input_->PushMouseButton(MouseButton::LeftButton)) {
		//銃弾を発射したフラグをオン
		isBulletFire_ = true;
		//間隔計測用タイマーをセット
		bulletFireIntervalTimer_ = bulletFireIntervalTime_;
		//現在の銃弾数を減らす
		bulletNum_--;
		//銃弾数が0になったらリロードタイマーをセット
		if (bulletNum_ <= 0) {
			bulletReloadTimer_ = bulletReloadTime_;
		}
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
	//死亡していたら処理をしない
	if (isDead_ && GetDeadTimer() > 0.0f) return;

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
