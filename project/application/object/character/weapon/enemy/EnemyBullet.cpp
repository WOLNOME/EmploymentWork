#include "EnemyBullet.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Object3dManager.h"
#include "CombinedParticleManager.h"
#include "BulletTrailManager.h"

//アプリケーション
#include <application/ui/player/PlayerUI.h>
#include <application/object/character/weapon/enemy/collision/EnemyBulletCollider.h>

using namespace Norm;

void EnemyBullet::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/enemyBullet");
	lifeTimer_ = 0.0f;

	//オブジェクトの初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("black.png");
	object3d_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Enemy_Bullet"), Shape::kSphere);
	object3d_->SetTexture(textureHandle_);
	object3d_->SetIsDisplay(false);
	//ワールドトランスフォームの初期化
	worldTransform_.SetTranslate({ FLT_MAX,FLT_MAX,FLT_MAX });
	worldTransform_.SetScale({ 0.01f,0.01f,0.01f });

	//トレールエフェクトの生成・初期化
	trail_ = std::make_unique<BulletTrail>();
	trail_->Initialize(BulletTrailManager::GetInstance()->GenerateName("enemyBullet"), param_["trailMaxLength"], param_["trailLengthDecayValue"]);
	trail_->SetTexture(TextureManager::GetInstance()->LoadTexture("red.png"));

	//衝突エフェクトの生成・初期化
	hitEffect_ = std::make_unique<CombinedParticle>();
	hitEffect_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("EnemyBulletHitEffect"), "Bullet_Hit");

	//当たり判定の生成・初期化
	collider_ = std::make_unique<EnemyBulletCollider>(this);
	auto* enemyBulletCollider = dynamic_cast<EnemyBulletCollider*>(collider_.get());
	collider_->SetCollisionAttribute(CollisionAttribute::Nothingness);
	collider_->SetWorldTransform(&worldTransform_);
	enemyBulletCollider->SetRadius(param_["collisionRadiusSphere"]);

}

void EnemyBullet::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//仮死状態だったらアイドル状態にする
	if (state_ == State::kAsphyxia) {
		SetState(State::kIdle);
	}

	//アクティブでないなら更新しない
	if (state_ != State::kActive)
		return;

	//移動処理
	Move();

	//トレールポジションの設定
	trail_->SetPosition(worldTransform_.GetTranslate());
}

void EnemyBullet::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

	ImGui::Begin("敵銃弾");
	ImGui::End();

#endif // _DEBUG
}

void EnemyBullet::Spawn(const Vector3& _initPos, const Vector3& _targetPos) {
	//初期位置を保存
	worldTransform_.SetTranslate(_initPos);
	generatedPosition_ = _initPos;
	//速度を算出
	float speed = param_["speed"];
	//向きを算出
	Vector3 direction = _targetPos - _initPos;
	//速度を更新
	velocity_ = direction * speed;
	//重力は計算しない
	gravity_ = 0.0f;
	//当たり判定を入力
	collider_->SetCollisionAttribute(CollisionAttribute::EnemyBullet);
	//アクティブ状態に変更
	state_ = State::kActive;
	//トレールの座標をクリア
	trail_->ClearPositions();
}

void EnemyBullet::DeadProcess() {
	//衝突エフェクトの発生
	TransformEuler transform = hitEffect_->GetBaseTransform();
	transform.translate = worldTransform_.GetTranslate();
	hitEffect_->SetBaseTransform(transform);
	hitEffect_->SetIsPlay(true);
	//仮死状態にする
	SetState(State::kAsphyxia);
	//寿命タイマーをリセット
	lifeTimer_ = 0.0f;
}

void EnemyBullet::Move() {
	//移動量の大きさを制限
	float maxSpeed = param_["maxSpeed"];
	if (velocity_.Length() > maxSpeed) {
		velocity_.Normalize();
		velocity_ *= maxSpeed;
	}

	//新座標を定義
	Vector3 newTranslate = worldTransform_.GetTranslate();
	newTranslate += velocity_ * kDeltaTime;

	//弾が地面に当たったら死亡
	if (newTranslate.y < 0.0f) {
		newTranslate.y = 0.0f;
		//死亡処理
		DeadProcess();
	}

	//座標をセット
	worldTransform_.SetTranslate(newTranslate);

	//弾が寿命を迎えたら死亡
	float lifeTime = param_["lifeTime"];
	lifeTimer_ += kDeltaTime;
	if (lifeTimer_ >= lifeTime) {
		//アイドル状態にする
		SetState(State::kIdle);
		//寿命タイマーをリセット
		lifeTimer_ = 0.0f;
	}
}
