#include "PlayerBullet.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Object3dManager.h"
#include "CombinedParticleManager.h"
#include "BulletTrailManager.h"

//アプリケーション
#include <application/object/character/weapon/player/collision/PlayerBulletCollider.h>

using namespace Norm;

void PlayerBullet::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerBullet");

	//インスタンスの生成と初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("black.png");
	object3d_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Player_Bullet"), Shape::kSphere);
	object3d_->SetTexture(textureHandle_);
	object3d_->SetIsDisplay(false);
	//ワールドトランスフォームの初期化
	worldTransform_.SetTranslate({ FLT_MAX,FLT_MAX,FLT_MAX });
	worldTransform_.SetScale({ 0.01f,0.01f,0.01f });

	//トレールエフェクトの生成と初期化
	{
		trail_ = std::make_unique<BulletTrail>();
		trail_->Initialize(BulletTrailManager::GetInstance()->GenerateName("playerBullet"), param_["trailMaxLength"], param_["trailLengthDecayValue"]);
		trail_->SetTexture(TextureManager::GetInstance()->LoadTexture("yellow.png"));
	}
	//衝突エフェクトの生成と初期化
	{
		hitEffect_ = std::make_unique<CombinedParticle>();
		hitEffect_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("PlayerBulletHitEffect"), "Bullet_Hit");
	}

	//当たり判定の生成・初期化
	collider_ = std::make_unique<PlayerBulletCollider>(this);
	auto* playerBulletCollider = dynamic_cast<PlayerBulletCollider*>(collider_.get());
	collider_->SetCollisionAttribute(CollisionAttribute::Nothingness);
	collider_->SetWorldTransform(&worldTransform_);
	playerBulletCollider->SetRadius(param_["collisionRadiusSphere"]);

}

void PlayerBullet::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//仮死状態なら
	if (state_ == State::kAsphyxia) {
		//稼働中のパーティクルがあるならreturn
		if (hitEffect_->GetIsPlay()) {
			return;
		}

		//全ての演出処理が終わったらアイドル状態にする
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

void PlayerBullet::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

	ImGui::Begin("プレイヤー銃弾");
	ImGui::End();

#endif // _DEBUG
}

void PlayerBullet::Spawn(const Vector3& _initPos, const Vector3& _initDirection) {
	//ステートがアイドルでなければ失敗
	if (state_ != State::kIdle) {
		return;
	}

	//初期位置を保存
	worldTransform_.SetTranslate(_initPos);
	//ワールドトランスフォームを更新（前データの上書き）
	worldTransform_.UpdateMatrix();
	//速度を算出
	float speed = param_["speed"];
	//速度を更新
	velocity_ = _initDirection * speed;
	//重力は計算しない
	gravity_ = 0.0f;
	//当たり判定を入力
	collider_->SetCollisionAttribute(CollisionAttribute::PlayerBullet);
	//アクティブ状態に変更
	state_ = State::kActive;
	//トレールの座標をクリア
	trail_->ClearPositions();
}

void PlayerBullet::DeadProcess() {
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

void PlayerBullet::Move() {
	//移動量の大きさを制限
	float maxSpeed = param_["maxSpeed"];
	if (velocity_.Length() > maxSpeed) {
		velocity_.Normalize();
		velocity_ *= maxSpeed;
	}

	//新トランスフォーム
	Vector3 newTranslate = worldTransform_.GetTranslate();

	newTranslate += velocity_ * kDeltaTime;

	//弾が地面に当たったら死亡
	if (newTranslate.y < 0.0f) {
		newTranslate.y = 0.0f;
		//死亡処理
		DeadProcess();
	}

	//弾が寿命を迎えたら死亡
	float lifeTime = param_["lifeTime"];
	lifeTimer_ += kDeltaTime;
	if (lifeTimer_ >= lifeTime) {
		//アイドル状態にする
		SetState(State::kIdle);
		//寿命タイマーをリセット
		lifeTimer_ = 0.0f;
	}

	//新トランスフォームをセット
	worldTransform_.SetTranslate(newTranslate);

}
