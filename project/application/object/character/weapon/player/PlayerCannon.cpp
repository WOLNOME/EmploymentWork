#include "PlayerCannon.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Object3dManager.h"
#include "CombinedParticleManager.h"
#include "BulletTrailManager.h"

//アプリケーション
#include <application/object/character/weapon/player/collision/PlayerCannonCollider.h>

using namespace Norm;

void PlayerCannon::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerCannon");

	//インスタンスの生成と初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("black.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Player_Cannon"), Shape::kSphere);
	object3d_->worldTransform.SetTranslate({ FLT_MAX,FLT_MAX ,FLT_MAX });
	object3d_->SetTexture(textureHandle_);
	//パーティクルの生成と初期化
	explosionParticle_ = std::make_unique<CombinedParticle>();
	explosionParticle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("PlayerCannonHit"), "Cannon_Hit");
	explosionParticle_->SetIsPlay(false);
	//トレールの生成と初期化
	trail_ = std::make_unique<BulletTrail>();
	trail_->Initialize(BulletTrailManager::GetInstance()->GenerateName("playerCannon"), param_["trailMaxLength"], param_["trailLengthDecayValue"]);
	trail_->SetTexture(TextureManager::GetInstance()->LoadTexture("yellow.png"));
	trail_->SetIsDisplay(false);

	//当たり判定の生成・初期化
	collider_ = std::make_unique<PlayerCannonCollider>(this);
	auto* playerCannonCollider = dynamic_cast<PlayerCannonCollider*>(collider_.get());
	collider_->SetCollisionAttribute(CollisionAttribute::Nothingness);
	collider_->SetWorldTransform(&object3d_->worldTransform);
	playerCannonCollider->SetRadius(param_["collisionRadiusSphere"]);

	//影の初期化
	circleShadow_->worldTransform.SetScale({ 1.0f,1.0f,1.0f });

}

void PlayerCannon::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//死亡演出が終了していたらアイドル状態にする
	if (state_ == State::kAsphyxia && !explosionParticle_->GetIsPlay()) {
		SetState(State::kIdle);
	}

	//弾がアクティブでないなら更新しない
	if (state_ != State::kActive)
		return;

	//移動処理
	Move();

	//トレール座標設定
	trail_->SetPosition(object3d_->worldTransform.GetTranslate());
}

void PlayerCannon::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

	ImGui::Begin("プレイヤーキャノン");
	ImGui::End();

#endif // _DEBUG

}

void PlayerCannon::Spawn(const Vector3& _initPos, const Vector3& _initDirection) {
	//ステートがアイドルでなければ失敗
	if (state_ != State::kIdle) {
		return;
	}

	//初期位置を保存
	object3d_->worldTransform.SetTranslate(_initPos);
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
	Vector3 newTranslate = object3d_->worldTransform.GetTranslate();
	newTranslate += velocity_ * kDeltaTime;

	//弾が地面に当たったら死亡
	if (newTranslate.y < 0.0f) {
		newTranslate.y = 0.0f;
		//アイドル状態にする
		SetState(State::kIdle);
	}
	//オブジェクトに反映
	object3d_->worldTransform.SetTranslate(newTranslate);
}
