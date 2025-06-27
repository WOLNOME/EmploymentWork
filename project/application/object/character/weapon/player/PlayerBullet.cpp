#include "PlayerBullet.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "ParticleManager.h"
#include "Object3dManager.h"

void PlayerBullet::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//インスタンスの生成と初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("black.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Player_Bullet"), Shape::kSphere);
	object3d_->worldTransform.scale = { 0.01f,0.01f,0.01f };
	object3d_->SetTexture(textureHandle_);
	//パーティクルの生成と初期化
	trail_ = std::make_unique<Particle>();
	trail_->Initialize(ParticleManager::GetInstance()->GenerateName("playerBulletTrail"), "trail");
	trail_->emitter_.isPlay = false;
	trail_->emitter_.transform.scale = { 0.01f,0.01f,0.01f };
	trail_->emitter_.generateMethod = Particle::GenerateMethod::Random;
	trail_->emitter_.effectStyle = Particle::EffectStyle::Loop;

	//当たり判定の形状を設定
	collisionShapeKind_ = CollisionShapeKind::Sphere;
	//当たり判定の半径を設定
	collisionRadius_ = 0.01f;

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerBullet");
	lifeTime_ = param_["lifeTime"];
	lifeTimer_ = 0.0f;

	//初期化時点では死亡状態
	isDead_ = true;

}

void PlayerBullet::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();
	//弾が死亡していたら更新しない
	if (GetDeadTimer() > 0.0f || GetIsDead()) return;

	//移動処理
	Move();
	//パーティクルの更新
	UpdateParticle();
}

void PlayerBullet::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

	ImGui::Begin("プレイヤー銃弾");
	ImGui::DragFloat3("座標", &object3d_->worldTransform.translate.x, 0.01f);
	ImGui::End();

	//デバッグ用ラインのカラー
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

#endif // _DEBUG

}

void PlayerBullet::OnCollision(CollisionAttribute attribute) {
	//当たり判定時の処理
	switch (attribute) {
		//敵に当たった場合
	case CollisionAttribute::Enemy:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//死亡処理
		DeadProcess();

		break;
		//敵弾に当たった場合
	case CollisionAttribute::EnemyBullet:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//死亡処理
		DeadProcess();

		break;
		//敵キャノンに当たった場合
	case CollisionAttribute::EnemyCannon:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//死亡処理
		DeadProcess();

		break;
	default:
		break;
	}
}

void PlayerBullet::SetInitParam(const Vector3& _initPos, const Vector3& _initDirection) {
	object3d_->worldTransform.translate = _initPos;
	float speed = param_["speed"];
	velocity_ = _initDirection * speed;
	gravity_ = 0.0f;
	SetCollisionAttribute(CollisionAttribute::PlayerBullet);
	isDead_ = false;
	trail_->emitter_.isPlay = true;	//トレイルパーティクルを開始
	trail_->emitter_.transform.translate = object3d_->worldTransform.translate;
	prePosition_ = { FLT_MAX,FLT_MAX ,FLT_MAX };
}

void PlayerBullet::Move() {
	//移動量の大きさを制限
	float maxSpeed = param_["maxSpeed"];
	if (velocity_.Length() > maxSpeed) {
		velocity_.Normalize();
		velocity_ *= maxSpeed;
	}
	object3d_->worldTransform.translate += velocity_ * kDeltaTime;

	//弾が地面に当たったら死亡
	if (GetDeadTimer() == 0.0f && !isDead_) {
		if (object3d_->worldTransform.translate.y < 0.0f) {
			object3d_->worldTransform.translate.y = 0.0f;
			//死亡処理
			DeadProcess();
		}
	}

	//弾が寿命を迎えたら死亡
	lifeTimer_ += kDeltaTime;
	if (lifeTimer_ >= lifeTime_) {
		if (GetDeadTimer() == 0.0f && !isDead_) {
			//死亡処理
			DeadProcess();
		}
	}
}

void PlayerBullet::UpdateParticle() {
	//トレイルパーティクルの座標更新
	trail_->emitter_.transform.translate = object3d_->worldTransform.worldTranslate;
}

void PlayerBullet::DeadProcess() {
	//死亡予約処理
	SetDeadTimer(0.1f);
	//当たり判定属性をなしに
	SetCollisionAttribute(CollisionAttribute::Nothingness);

	//パーティクルの制御
	trail_->emitter_.isPlay = false;	//トレイルパーティクルを停止

	lifeTimer_ = 0.0f;	//寿命タイマーをリセット
}
