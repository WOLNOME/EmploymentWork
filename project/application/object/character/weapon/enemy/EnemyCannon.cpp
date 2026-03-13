#include "EnemyCannon.h"
#include <TextureManager.h>
#include <ImGuiManager.h>
#include <CombinedParticleManager.h>
#include <Object3dManager.h>
#include "BulletTrailManager.h"

//アプリケーション
#include <application/ui/player/PlayerUI.h>
#include <application/object/character/weapon/enemy/collision/EnemyCannonCollider.h>
#include <application/system/CameraManager.h>

using namespace Norm;

void EnemyCannon::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//SEの初期化
	shotSE_ = std::make_unique<Audio>();
	shotSE_->Initialize("se/cannonShot.mp3");
	deadSE_ = std::make_unique<Audio>();
	deadSE_->Initialize("se/explosion_small.mp3");

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/EnemyCannon");
	audioParam_ = JsonUtil::GetJsonData("Resources/parameters/audio");

	//インスタンスの初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("red.png");
	object3d_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Enemy_Cannon"), Shape::kSphere);
	object3d_->SetIsDisplay(false);
	object3d_->SetTexture(textureHandle_);
	//ワールドトランスフォームの初期化
	worldTransform_.SetTranslate({ FLT_MAX,FLT_MAX,FLT_MAX });

	//爆発パーティクルの生成と初期化
	{
		explosionParticle_ = std::make_unique<CombinedParticle>();
		explosionParticle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("EnemyCannonHit"), "Cannon_Hit");
		explosionParticle_->SetIsPlay(false);
	}
	//地面衝突パーティクルの生成と初期化
	{
		groundParticle_ = std::make_unique<CombinedParticle>();
		groundParticle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("EnemyCannonGround"), "Cannon_Ground");
		explosionParticle_->SetIsPlay(false);
	}
	//発射時パーティクルの生成と初期化
	{
		fireParticle_ = std::make_unique<CombinedParticle>();
		fireParticle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("EnemyCannonFire"), "Cannon_Fire");
		fireParticle_->SetIsPlay(false);
	}
	//トレールの生成と初期化
	{
		trail_ = std::make_unique<BulletTrail>();
		trail_->Initialize(BulletTrailManager::GetInstance()->GenerateName("EnemyCannon"), param_["trailMaxLength"], param_["trailLengthDecayValue"]);
		trail_->SetTexture(TextureManager::GetInstance()->LoadTexture("red.png"));
	}

	//当たり判定の生成・初期化
	collider_ = std::make_unique<EnemyCannonCollider>(this);
	auto* enemyCannonCollider = dynamic_cast<EnemyCannonCollider*>(collider_.get());
	collider_->SetCollisionAttribute(CollisionAttribute::Nothingness);
	collider_->SetWorldTransform(&worldTransform_);
	enemyCannonCollider->SetRadius(param_["collisionRadiusSphere"]);

	//影の初期化
	csWorldTransform_.SetScale({ 1.0f,1.0f,1.0f });
}

void EnemyCannon::Update() {
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
	if (GetState() != State::kActive)
		return;

	//移動処理
	Move();

	//トレール座標設定
	trail_->SetPosition(worldTransform_.GetTranslate());
}

void EnemyCannon::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

	ImGui::Begin("敵キャノン");
	ImGui::End();

#endif // _DEBUG
}

void EnemyCannon::Spawn(const Vector3& _initPos, const Vector3& _targetPos, float _size) {
	//ステートがアイドルでなければ失敗
	if (state_ != State::kIdle) {
		return;
	}

	//初期位置を保存
	worldTransform_.SetTranslate(_initPos);
	generatedPosition_ = _initPos;
	//サイズを保存
	worldTransform_.SetScale({ _size ,_size,_size });
	csWorldTransform_.SetScale({ _size ,_size,_size });
	auto* enemyCannonCollider = dynamic_cast<EnemyCannonCollider*>(collider_.get());
	enemyCannonCollider->SetRadius(_size);
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
	//向きベクトルを算出
	Vector3 targetVec = _targetPos - _initPos;
	//XZ方向の速度を算出
	float hitTime = param_["hitTime"];
	velocity_.x = targetVec.x / hitTime;
	velocity_.z = targetVec.z / hitTime;
	//トレールエフェクトをクリア
	trail_->ClearPositions();
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
	//発射SE
	shotSE_->Play(false, volume);

	//最大高度から重力を求める
	float maxHeight = param_["maxHeight"];
	//もしinitPosのy座標がmaxHeightより大きい場合
	if (_initPos.y > maxHeight) {
		//最大高度をinitPosのy座標にする
		maxHeight = _initPos.y;
		gravity_ = 2.0f * (_initPos.y - _targetPos.y) / std::powf(hitTime, 2);
		//y方向の上昇速度は0
		velocity_.y = 0.0f;
		//当たり判定属性をセット
		collider_->SetCollisionAttribute(CollisionAttribute::EnemyCannon);
		//アクティブ状態にする
		SetState(State::kActive);

		return;
	}

	gravity_ = 2.0f * (maxHeight - _targetPos.y) / std::powf((hitTime / 2.0f), 2);
	//y方向の上昇速度を算出
	velocity_.y = 4.0f * (_initPos.y - _targetPos.y) / hitTime;

	//当たり判定属性をセット
	collider_->SetCollisionAttribute(CollisionAttribute::EnemyCannon);
	//アクティブ状態にする
	SetState(State::kActive);
}

void EnemyCannon::DeadProcess(DeadType _deadType) {

	//タイプ別の処理
	switch (_deadType) {
	case EnemyCannon::DeadType::Collide:
	{
		//爆発パーティクルの発生
		TransformEuler transform = explosionParticle_->GetBaseTransform();
		transform.translate = worldTransform_.GetTranslate();
		explosionParticle_->SetBaseTransform(transform);
		explosionParticle_->SetIsPlay(true);

		break;
	}
	case EnemyCannon::DeadType::Ground:
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

void EnemyCannon::Move() {
	//重力をかける
	velocity_.y -= gravity_ * kDeltaTime;

	//新座標を定義
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
