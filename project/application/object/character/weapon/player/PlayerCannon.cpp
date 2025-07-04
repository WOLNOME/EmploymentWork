#include "PlayerCannon.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Object3dManager.h"
#include "ParticleManager.h"
#include "BulletTrailManager.h"

void PlayerCannon::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//インスタンスの生成と初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("black.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{},Object3dManager::GetInstance()->GenerateName("Player_Cannon"), Shape::kSphere);
	object3d_->SetTexture(textureHandle_);
	//パーティクルの生成と初期化
	particle_ = std::make_unique<Particle>();
	particle_->Initialize(ParticleManager::GetInstance()->GenerateName("playerCannonHit"), "hit");
	particle_->emitter_.isPlay = false;
	particle_->emitter_.transform.scale = { 0.1f,0.1f,0.1f };
	particle_->emitter_.generateMethod = Particle::GenerateMethod::Clump;
	particle_->emitter_.clumpNum = 10;
	particle_->emitter_.effectStyle = Particle::EffectStyle::OneShot;
	//トレールの生成と初期化
	trail_ = std::make_unique<BulletTrail>();
	trail_->Initialize(BulletTrailManager::GetInstance()->GenerateName("playerCannon"));
	trail_->SetTexture(TextureManager::GetInstance()->LoadTexture("yellow.png"));

	//当たり判定の形状を設定
	collisionShapeKind_ = Collider::CollisionShapeKind::Sphere;
	//当たり判定の半径を設定
	collisionRadius_ = 1.0f;

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerCannon");

	//初期化時点では死亡状態
	isDead_ = true;

}

void PlayerCannon::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();
	//弾が死亡していたら更新しない
	if (GetDeadTimer() > 0.0f || GetIsDead()) return;
	

	//移動処理
	Move();

	//トレール座標設定
	trail_->SetPosition(object3d_->worldTransform.translate);
}

void PlayerCannon::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

	ImGui::Begin("プレイヤーキャノン");
	ImGui::DragFloat3("座標", &object3d_->worldTransform.translate.x, 0.01f);
	ImGui::End();

	//デバッグ用ラインのカラー
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

#endif // _DEBUG

}

void PlayerCannon::OnCollision(CollisionAttribute attribute) {
	//当たり判定時の処理
	switch (attribute) {
		//敵に当たった場合
	case CollisionAttribute::Enemy:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//パーティクルの発生
		particle_->emitter_.transform.translate = object3d_->worldTransform.worldTranslate;
		particle_->emitter_.isPlay = true;
		//死亡予約処理
		SetDeadTimer(particle_->GetParam()["LifeTime"]["Max"]);
		//当たり判定属性をなしに
		SetCollisionAttribute(CollisionAttribute::Nothingness);

		break;
		//敵弾に当たった場合
	case CollisionAttribute::EnemyBullet:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//パーティクルの発生
		particle_->emitter_.transform.translate = object3d_->worldTransform.worldTranslate;
		particle_->emitter_.isPlay = true;
		//死亡予約処理
		SetDeadTimer(particle_->GetParam()["LifeTime"]["Max"]);
		//当たり判定属性をなしに
		SetCollisionAttribute(CollisionAttribute::Nothingness);

		break;
		//敵キャノンに当たった場合
	case CollisionAttribute::EnemyCannon:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//パーティクルの発生
		particle_->emitter_.transform.translate = object3d_->worldTransform.worldTranslate;
		particle_->emitter_.isPlay = true;
		//死亡予約処理
		SetDeadTimer(particle_->GetParam()["LifeTime"]["Max"]);
		//当たり判定属性をなしに
		SetCollisionAttribute(CollisionAttribute::Nothingness);

		break;
	default:
		break;
	}
}

void PlayerCannon::SetInitParam(const Vector3& _initPos, const Vector3& _initDirection) {
	object3d_->worldTransform.translate = _initPos;
	float speed = param_["speed"];
	velocity_ = _initDirection * speed;
	SetCollisionAttribute(CollisionAttribute::PlayerCannon);
	isDead_ = false;
	prePosition_ = { FLT_MAX,FLT_MAX ,FLT_MAX };
}

void PlayerCannon::Move() {
	//重力をかける
	velocity_.y -= gravity_ * kDeltaTime;
	//空気抵抗をかける
	Vector3 airResistanceDir = -velocity_.Normalized();
	Vector3 airResistanceAccel = airResistanceDir * airResistance_ * velocity_.Length();
	velocity_ += airResistanceAccel * kDeltaTime;
	//移動量の大きさを制限
	float maxSpeed = param_["maxSpeed"];
	if (velocity_.Length() > maxSpeed) {
		velocity_.Normalize();
		velocity_ *= maxSpeed;
	}
	object3d_->worldTransform.translate += velocity_ * kDeltaTime;

	//弾が地面に当たったら死亡
	if (GetDeadTimer() == 0.0f) {
		if (object3d_->worldTransform.translate.y < 0.0f) {
			object3d_->worldTransform.translate.y = 0.0f;
			//死亡予約処理
			SetDeadTimer(0.1f);
			//当たり判定属性をなしに
			SetCollisionAttribute(CollisionAttribute::Nothingness);
		}
	}

}
