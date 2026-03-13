#include "PlayerCannon.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Object3dManager.h"
#include "CombinedParticleManager.h"
#include "BulletTrailManager.h"

//アプリケーション
#include <application/object/character/weapon/player/collision/PlayerCannonCollider.h>
#include <application/system/CameraManager.h>

using namespace Norm;

void PlayerCannon::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//SEの初期化
	shotSE_ = std::make_unique<Audio>();
	shotSE_->Initialize("se/cannonShot.mp3");
	deadSE_ = std::make_unique<Audio>();
	deadSE_->Initialize("se/explosion_small.mp3");

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerCannon");
	audioParam_ = JsonUtil::GetJsonData("Resources/parameters/audio");

	//インスタンスの生成と初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("black.png");
	object3d_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Player_Cannon"), Shape::kSphere);
	object3d_->SetTexture(textureHandle_);
	//ワールドトランスフォームの初期化
	worldTransform_.SetTranslate({ FLT_MAX,FLT_MAX ,FLT_MAX });

	//爆発パーティクルの生成と初期化
	{
		explosionParticle_ = std::make_unique<CombinedParticle>();
		explosionParticle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("PlayerCannonHit"), "Cannon_Hit");
		explosionParticle_->SetIsPlay(false);
	}
	//地面衝突パーティクルの生成と初期化
	{
		groundParticle_ = std::make_unique<CombinedParticle>();
		groundParticle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("PlayerCannonGround"), "Cannon_Ground");
		explosionParticle_->SetIsPlay(false);
	}
	//発射時パーティクルの生成と初期化
	{
		fireParticle_ = std::make_unique<CombinedParticle>();
		fireParticle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("PlayerCannonFire"), "Cannon_Fire");
		fireParticle_->SetIsPlay(false);
	}
	//トレールの生成と初期化
	{
		trail_ = std::make_unique<BulletTrail>();
		trail_->Initialize(BulletTrailManager::GetInstance()->GenerateName("playerCannon"), param_["trailMaxLength"], param_["trailLengthDecayValue"]);
		trail_->SetTexture(TextureManager::GetInstance()->LoadTexture("yellow.png"));
	}
	//当たり判定の生成・初期化
	collider_ = std::make_unique<PlayerCannonCollider>(this);
	auto* playerCannonCollider = dynamic_cast<PlayerCannonCollider*>(collider_.get());
	collider_->SetCollisionAttribute(CollisionAttribute::Nothingness);
	collider_->SetWorldTransform(&worldTransform_);
	playerCannonCollider->SetRadius(param_["collisionRadiusSphere"]);

	//影の初期化
	csWorldTransform_.SetScale({ 1.0f,1.0f,1.0f });

}

void PlayerCannon::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//仮死状態だったら
	if (state_ == State::kAsphyxia) {
		//パーティクルが稼働中ならreturn
		if (explosionParticle_->GetIsPlay() || groundParticle_->GetIsPlay() || fireParticle_->GetIsPlay()) {
			return;
		}

		//全ての演出処理が終わったらアイドル状態にする
		SetState(State::kIdle);
	}

	//弾がアクティブでないなら更新しない
	if (state_ != State::kActive)
		return;

	//移動処理
	Move();

	//トレール座標設定
	trail_->SetPosition(worldTransform_.GetTranslate());
}

void PlayerCannon::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

#endif // _DEBUG

}

void PlayerCannon::Spawn(const Vector3& _initPos, const Vector3& _initDirection) {
	//ステートがアイドルでなければ失敗
	if (state_ != State::kIdle) {
		return;
	}

	//初期位置を保存
	worldTransform_.SetTranslate(_initPos);
	//ワールドトランスフォームを更新（前データの上書き）
	worldTransform_.UpdateMatrix();
	//発射時パーティクルの発生
	TransformEuler transform = fireParticle_->GetBaseTransform();
	transform.translate = _initPos;
	fireParticle_->SetBaseTransform(transform);
	fireParticle_->SetIsPlay(true);
	//表示する
	object3d_->SetIsDisplay(true);
	circleShadow_->SetIsDisplay(true);
	//速度を算出
	float speed = param_["speed"];
	//速度を決める
	velocity_ = _initDirection * speed;
	//当たり判定の属性を決める
	collider_->SetCollisionAttribute(CollisionAttribute::PlayerCannon);
	//アクティブ状態にする
	SetState(State::kActive);
	//トレールの座標をクリア
	trail_->ClearPositions();
	//発射SE
	shotSE_->Play(false, 1.0f);
}

void PlayerCannon::DeadProcess(DeadType _deadType) {

	//タイプ別の処理
	switch (_deadType) {
	case PlayerCannon::DeadType::Collide:
	{
		//爆発パーティクルの発生
		TransformEuler transform = explosionParticle_->GetBaseTransform();
		transform.translate = worldTransform_.GetTranslate();
		explosionParticle_->SetBaseTransform(transform);
		explosionParticle_->SetIsPlay(true);
		
		break;
	}
	case PlayerCannon::DeadType::Ground:
	{
		//地面衝突パーティクルの発生
		TransformEuler transform = groundParticle_->GetBaseTransform();
		transform.translate = worldTransform_.GetTranslate();
		groundParticle_->SetBaseTransform(transform);
		groundParticle_->SetIsPlay(true);
		
		break;
	}
	default:
		break;
	}

	//最大距離
	float maxDistance = audioParam_["distance"].get<float>();
	//カメラまでの距離
	float distance = Vector3(
		worldTransform_.GetTranslate() - cameraManager_->GetActiveCamera()->worldTransform.GetWorldTranslate()
	).Length();
	//音量
	float volume = 0.0f;
	if (distance < maxDistance) {
		volume = MyMath::Lerp(1.0f, 0.0f, distance / maxDistance);
	}
	//死亡SE
	deadSE_->Play(false, volume);
	//仮死状態にする
	SetState(BaseCharacter::State::kAsphyxia);

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
	Vector3 newTranslate = worldTransform_.GetTranslate();
	newTranslate += velocity_ * kDeltaTime;

	//弾が地面に当たったら死亡
	if (newTranslate.y < 0.0f) {
		newTranslate.y = 0.0f;
		worldTransform_.SetTranslate(newTranslate);
		DeadProcess(DeadType::Ground);
	}
	else {
		worldTransform_.SetTranslate(newTranslate);
	}
}
