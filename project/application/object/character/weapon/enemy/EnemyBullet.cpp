#include "EnemyBullet.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "Object3dManager.h"
#include "CombinedParticleManager.h"
#include "BulletTrailManager.h"

//アプリケーション
#include <application/ui/player/PlayerUI.h>

using namespace Norm;

void EnemyBullet::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/enemyBullet");
	lifeTimer_ = 0.0f;

	//インスタンスの生成と初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("black.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Enemy_Bullet"), Shape::kSphere);
	object3d_->worldTransform.SetTranslate({ FLT_MAX,FLT_MAX,FLT_MAX });
	object3d_->worldTransform.SetScale({ 0.01f,0.01f,0.01f });
	object3d_->SetTexture(textureHandle_);
	object3d_->SetIsDisplay(false);
	//トレールエフェクトの生成と初期化
	trail_ = std::make_unique<BulletTrail>();
	trail_->Initialize(BulletTrailManager::GetInstance()->GenerateName("enemyBullet"), param_["trailMaxLength"], param_["trailLengthDecayValue"]);
	trail_->SetTexture(TextureManager::GetInstance()->LoadTexture("red.png"));
	//衝突エフェクトの生成と初期化
	hitEffect_ = std::make_unique<CombinedParticle>();
	hitEffect_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("EnemyBulletHitEffect"), "Bullet_Hit");

	//当たり判定の形状を設定
	collisionShapeKind_ = CollisionShapeKind::Sphere;
	//当たり判定の半径を設定
	collisionRadius_ = 0.01f;
}

void EnemyBullet::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//死亡演出が終了したらアイドル状態にする
	if (state_ == State::kAsphyxia && !hitEffect_->GetIsPlay()) {
		SetState(State::kIdle);
	}

	//アクティブでないなら更新しない
	if (state_ != State::kActive)
		return;

	//移動処理
	Move();

	//トレールポジションの設定
	trail_->SetPosition(object3d_->worldTransform.GetTranslate());
}

void EnemyBullet::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

	ImGui::Begin("敵銃弾");
	ImGui::End();

	//デバッグ用ラインのカラー
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

#endif // _DEBUG
}

void EnemyBullet::Spawn(const Vector3& _initPos, const Vector3& _targetPos) {
	//初期位置を保存
	object3d_->worldTransform.SetTranslate(_initPos);
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
	SetCollisionAttribute(CollisionAttribute::EnemyBullet);
	//アクティブ状態に変更
	state_ = State::kActive;
	prePosition_ = { FLT_MAX,FLT_MAX ,FLT_MAX };
	trail_->ClearPositions();
}

void EnemyBullet::OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) {
	//当たり判定時の処理
	switch (attribute) {
		//プレイヤーに当たった場合
	case CollisionAttribute::Player:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//死亡処理
		DeadProcess();

		//被弾インジケーターをつける
		playerUI_->GetHitIndicator()->RegistIndicator(generatedPosition_);

		break;
		//プレイヤー砲弾に当たった場合
	case CollisionAttribute::PlayerCannon:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//死亡処理
		DeadProcess();

		break;
	default:
		break;
	}
}

void EnemyBullet::Move() {
	//移動量の大きさを制限
	float maxSpeed = param_["maxSpeed"];
	if (velocity_.Length() > maxSpeed) {
		velocity_.Normalize();
		velocity_ *= maxSpeed;
	}

	//新座標を定義
	Vector3 newTranslate = object3d_->worldTransform.GetTranslate();
	newTranslate += velocity_ * kDeltaTime;

	//弾が地面に当たったら死亡
	if (newTranslate.y < 0.0f) {
		newTranslate.y = 0.0f;
		//死亡処理
		DeadProcess();
	}

	//座標をセット
	object3d_->worldTransform.SetTranslate(newTranslate);

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

void EnemyBullet::DeadProcess() {
	//衝突エフェクトの発生
	TransformEuler transform = hitEffect_->GetBaseTransform();
	transform.translate = object3d_->worldTransform.GetTranslate();
	hitEffect_->SetBaseTransform(transform);
	hitEffect_->SetIsPlay(true);
	//仮死状態にする
	SetState(State::kAsphyxia);
	//寿命タイマーをリセット
	lifeTimer_ = 0.0f;
}
