#include "EnemyCannon.h"
#include <TextureManager.h>
#include <ImGuiManager.h>
#include <CombinedParticleManager.h>
#include <Object3dManager.h>
#include "BulletTrailManager.h"

//アプリケーション
#include <application/ui/player/PlayerUI.h>
#include <application/object/character/weapon/enemy/collision/EnemyCannonCollider.h>

using namespace Norm;

void EnemyCannon::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/EnemyCannon");

	//インスタンスの生成と初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("red.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Enemy_Cannon"), Shape::kSphere);
	object3d_->worldTransform.SetTranslate({ FLT_MAX,FLT_MAX,FLT_MAX });
	object3d_->SetIsDisplay(false);
	object3d_->SetTexture(textureHandle_);
	//トレールエフェクトの生成と初期化
	trail_ = std::make_unique<BulletTrail>();
	trail_->Initialize(BulletTrailManager::GetInstance()->GenerateName("enemyCannon"), param_["trailMaxLength"], param_["trailLengthDecayValue"]);
	trail_->SetTexture(TextureManager::GetInstance()->LoadTexture("red.png"));
	//パーティクルの生成と初期化
	explosionParticle_ = std::make_unique<CombinedParticle>();
	explosionParticle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("EnemyCannonHit"), "Cannon_Hit");

	//当たり判定の生成・初期化
	collider_ = std::make_unique<EnemyCannonCollider>(this);
	auto* enemyCannonCollider = dynamic_cast<EnemyCannonCollider*>(collider_.get());
	collider_->SetCollisionAttribute(CollisionAttribute::Nothingness);
	collider_->SetWorldTransform(&object3d_->worldTransform);
	enemyCannonCollider->SetRadius(param_["collisionRadiusSphere"]);

	//影の初期化
	circleShadow_->worldTransform.SetScale({ 1.0f,1.0f,1.0f });
}

void EnemyCannon::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//死亡演出が終わったらアイドル状態にする
	if (state_ == State::kAsphyxia && !explosionParticle_->GetIsPlay()) {
		SetState(State::kIdle);
	}

	//弾がアクティブでないなら更新しない
	if (GetState() != State::kActive)
		return;

	//移動処理
	Move();

	//トレールポジションの設定
	trail_->SetPosition(object3d_->worldTransform.GetTranslate());
}

void EnemyCannon::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

	ImGui::Begin("敵キャノン");
	ImGui::End();

#endif // _DEBUG
}

void EnemyCannon::Spawn(const Vector3& _initPos, const Vector3& _targetPos) {
	//ステートがアイドルでなければ失敗
	if (state_ != State::kIdle) {
		return;
	}

	//初期位置を保存
	object3d_->worldTransform.SetTranslate(_initPos);
	generatedPosition_ = _initPos;
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

void EnemyCannon::Move() {
	//重力をかける
	velocity_.y -= gravity_ * kDeltaTime;

	//新座標を定義
	Vector3 newTranslate = object3d_->worldTransform.GetTranslate();
	newTranslate += velocity_ * kDeltaTime;

	//弾が地面に当たったら
	if (newTranslate.y < 0.0f) {
		newTranslate.y = 0.0f;
		//アイドル状態にする
		SetState(State::kIdle);
	}

	//座標をセット
	object3d_->worldTransform.SetTranslate(newTranslate);
}
