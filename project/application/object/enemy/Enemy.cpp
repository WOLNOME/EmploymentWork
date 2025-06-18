#include "Enemy.h"
#include "ImGuiManager.h"

//アプリケーション
#include "application/object/player/Player.h"

void Enemy::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();
	//インスタンスの生成と初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, "enemy");
	if (light_) {
		object3d_->SetSceneLight(light_);
	}

	//当たり判定の半径を設定
	radius_ = 3.8f;
	//当たり判定の属性を設定
	SetCollisionAttribute(CollisionAttribute::Enemy);

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/enemy");
	//パラメータの反映
	maxHP_ = param_["maxHP"];
	hp_ = maxHP_;

}

void Enemy::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//移動処理
	Move();
	//回転処理
	Rotate();
	//攻撃
	Attack();
	//死亡処理
	DeadProcess();

	//弾の更新処理
	UpdateBullets();

}

void Enemy::Draw() {
	//オブジェクトの描画
	object3d_->Draw(camera_);
	//弾の描画
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}
}

void Enemy::DrawLine() {
	//ベースキャラクターのライン描画
	BaseCharacter::DrawLine();
	//弾のライン描画
	for (auto& bullet : bullets_) {
		bullet->DrawLine();
	}
}

void Enemy::DebugWithImGui() {
#ifdef _DEBUG
	ImGui::Begin("敵");
	ImGui::DragFloat3("座標", &object3d_->worldTransform.translate.x, 0.01f);
	ImGui::End();

	//弾のデバッグ
	for (auto& bullet : bullets_) {
		bullet->DebugWithImGui();
	}

	//デバッグ用ラインのカラー
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

#endif // _DEBUG
}

void Enemy::OnCollision(CollisionAttribute attribute) {
	//当たり判定時の処理
	switch (attribute) {
		//プレイヤーに当たった場合
	case CollisionAttribute::Player:
		break;
		//プレイヤー弾に当たった場合
	case CollisionAttribute::PlayerBullet:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//HPを減らす
		hp_ -= 10;
		//0~MaxHPの範囲に収める
		hp_ = std::clamp(hp_, 0, maxHP_);


		break;
	default:
		break;
	}
}

void Enemy::SetPosition(const Vector3& _pos) {
	object3d_->worldTransform.translate = _pos;
	object3d_->worldTransform.translate.y = 2.7f;
}

void Enemy::Move() {
	//もしプレイヤーが索敵範囲内にいないなら処理を行わない
	if (player_->GetWorldTransform().translate.Distance(object3d_->worldTransform.translate) > param_["searchPlayerDistanceMove"]) {
		return;
	}
	//攻撃中は処理を行わない
	if (isEnemyAttacked_) {
		return;
	}

	//プレイヤーへの方向を求める
	Vector3 dirToPlayer = player_->GetWorldTransform().translate - object3d_->worldTransform.translate;
	//y座標は考慮しない
	dirToPlayer.y = 0.0f;
	//正規化
	dirToPlayer.Normalize();

	//移動量を求める
	float speed = param_["speed"];
	velocity_ += dirToPlayer * speed;
	//摩擦力をかける
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

void Enemy::Rotate() {
	//もしプレイヤーが索敵範囲内にいなければ処理を行わない。
	if (player_->GetWorldTransform().translate.Distance(object3d_->worldTransform.translate) > param_["searchPlayerDistanceRotate"]) {
		return;
	}
	//移動方向に向かって回転->現在の向きを求める
	Vector3 currentDir = {
		std::sinf(object3d_->worldTransform.rotate.y),
		0.0f,
		std::cosf(object3d_->worldTransform.rotate.y)
	};
	currentDir.Normalize();
	//プレイヤーへの方向を求める
	Vector3 dirToPlayer = player_->GetWorldTransform().translate - object3d_->worldTransform.translate;
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
	float rotateSpeed = param_["rotateSpeed"];
	if (std::abs(angle) < rotateSpeed * kDeltaTime) {
		usingRotateSpeed = angle;
	}
	else {
		//回転スピードを使う場合、符号を揃える
		usingRotateSpeed = (angle > 0) ? rotateSpeed * kDeltaTime : -rotateSpeed * kDeltaTime;
	}
	//ワールドトランスフォームの回転を加算
	object3d_->worldTransform.rotate.y += usingRotateSpeed;
	//回転の範囲を-PI~PIに収める(Clampは×)
	if (object3d_->worldTransform.rotate.y > pi) {
		object3d_->worldTransform.rotate.y -= 2.0f * pi;
	}
	else if (object3d_->worldTransform.rotate.y < -pi) {
		object3d_->worldTransform.rotate.y += 2.0f * pi;
	}
}

void Enemy::Attack() {
	//クールタイム処理
	if (isEnemyAttacked_) {
		attackCoolTimer_ += kDeltaTime;
		if (attackCoolTimer_ >= param_["attackCoolTime"]) {
			isEnemyAttacked_ = false;
			attackCoolTimer_ = 0.0f;
		}
	}
	//もしプレイヤーが索敵範囲内にいなければ処理を行わない。
	if (player_->GetWorldTransform().translate.Distance(object3d_->worldTransform.translate) > param_["searchPlayerDistanceAttack"]) {
		return;
	}
	//未攻撃状態なら攻撃処理
	if (!isEnemyAttacked_) {
		//車体の向きを求める
		Vector3 currentDir = {
			std::sinf(object3d_->worldTransform.rotate.y),
			0.0f,
			std::cosf(object3d_->worldTransform.rotate.y)
		};
		currentDir.Normalize();
		//弾のインスタンスを生成
		std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();
		bullet->Initialize();
		//セット
		bullet->SetCamera(camera_);
		bullet->SetSceneLight(light_);
		//初期位置と目標位置をセット
		Vector3 bulletPos = object3d_->worldTransform.translate;
		bulletPos.y += 2.0f;	//←高さ
		bulletPos.x += currentDir.x * 10.0f;
		bulletPos.z += currentDir.z * 10.0f;
		bullet->SetInitParam(bulletPos, player_->GetWorldPosition());
		//リストに追加
		bullets_.push_back(std::move(bullet));
		isEnemyAttacked_ = true;
	}

}

void Enemy::UpdateBullets() {
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

void Enemy::DeadProcess() {
	//もしHPが0になったら
	if (hp_ <= 0) {
		//死亡タイマーをセット
		SetDeadTimer(0.1f);
	}
}
