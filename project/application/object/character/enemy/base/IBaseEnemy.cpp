#include "IBaseEnemy.h"
#include "ImGuiManager.h"
#include "ParticleManager.h"

//アプリケーション
#include "application/object/character/player/Player.h"

void IBaseEnemy::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//当たり判定の形状を設定
	collisionShapeKind_ = CollisionShapeKind::AABB;
	//当たり判定の属性を設定
	SetCollisionAttribute(CollisionAttribute::Enemy);

	//パーティクルの生成・初期化
	deadParticle_ = std::make_unique<Particle>();
	deadParticle_->Initialize(ParticleManager::GetInstance()->GenerateName("EnemyDead"), "enemy_explosion");
	deadParticle_->emitter_.isPlay = false;
	deadParticle_->emitter_.transform.scale = { 0.1f,0.1f,0.1f };
	deadParticle_->emitter_.effectStyle = Particle::EffectStyle::Loop;
	deadParticle_->emitter_.isGravity = true;
	deadParticle_->emitter_.gravity = -50.0f;
	deadParticle_->emitter_.isBound = true;
	deadParticle_->emitter_.floorHeight = 0.0f;

}

void IBaseEnemy::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//移動処理
	Move();
	//回転処理
	Rotate();
	//死亡処理
	DeadProcess();
}

void IBaseEnemy::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

	ImGui::Begin("敵");
	ImGui::DragFloat3("座標", &object3d_->worldTransform.translate.x, 0.01f);
	ImGui::End();

	//デバッグ用ラインのカラー
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

#endif // _DEBUG
}

void IBaseEnemy::SetPosition(const Vector3& _pos) {
	object3d_->worldTransform.translate = _pos;
}

void IBaseEnemy::OnCollision(CollisionAttribute attribute) {
	//当たり判定時の処理
	switch (attribute) {
		//プレイヤーに当たった場合
	case CollisionAttribute::Player:
		break;
		//プレイヤーキャノンに当たった場合
	case CollisionAttribute::PlayerCannon:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//HPを減らす
		hp_ -= 10;
		//0~MaxHPの範囲に収める
		hp_ = std::clamp(hp_, 0, maxHP_);

		break;
		//プレイヤー弾に当たった場合
	case CollisionAttribute::PlayerBullet:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//HPを減らす
		hp_ -= 1;
		//0~MaxHPの範囲に収める
		hp_ = std::clamp(hp_, 0, maxHP_);

		break;
	default:
		break;
	}
}

void IBaseEnemy::Move() {
	//もしプレイヤーが索敵範囲内にいないなら処理を行わない
	float searchPlayerDistanceMove = param_["searchPlayerDistanceMove"];
	if (player_->GetWorldTransform().translate.Distance(object3d_->worldTransform.translate) > searchPlayerDistanceMove) {
		return;
	}
	//攻撃範囲内にいるなら処理を行わない
	float searchPlayerDistanceAttack = param_["searchPlayerDistanceAttack"];
	if (player_->GetWorldTransform().translate.Distance(object3d_->worldTransform.translate) < searchPlayerDistanceAttack) {
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

void IBaseEnemy::Rotate() {
	//もしプレイヤーが索敵範囲内にいなければ処理を行わない。
	float searchPlayerDistanceRotate = param_["searchPlayerDistanceRotate"];
	if (player_->GetWorldTransform().translate.Distance(object3d_->worldTransform.translate) > searchPlayerDistanceRotate) {
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

void IBaseEnemy::DeadProcess() {
	//もしHPが0になったら
	if (hp_ <= 0) {
		deadParticleOnTimer_ += kDeltaTime;
		//死亡パーティクルをオン
		deadParticle_->emitter_.transform.translate = object3d_->worldTransform.worldTranslate;
		deadParticle_->emitter_.isPlay = true;
		//時間を超えたらオフにする
		if (deadParticleOnTimer_ > deadParticleOnTime_) {
			deadParticle_->emitter_.isPlay = false;
		}

		//死亡タイマーをセット(設計上上書きされることはない)
		float particleLifeTime = deadParticle_->GetParam()["LifeTime"]["Max"];
		SetDeadTimer(float(deadParticleOnTime_ + particleLifeTime));
	}
}
