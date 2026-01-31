#include "EnemyCannon.h"
#include <TextureManager.h>
#include <ImGuiManager.h>
#include <CombinedParticleManager.h>
#include <Object3dManager.h>

//アプリケーション
#include <application/ui/player/PlayerUI.h>

void EnemyCannon::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//インスタンスの生成と初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("red.png");
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ShapeTag{}, Object3dManager::GetInstance()->GenerateName("Enemy_Cannon"), Shape::kSphere);
	object3d_->worldTransform.translate = { FLT_MAX,FLT_MAX ,FLT_MAX };
	object3d_->SetIsDisplay(false);
	object3d_->SetTexture(textureHandle_);
	//パーティクルの生成と初期化
	particle_ = std::make_unique<CombinedParticle>();
	particle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("EnemyCannonHit"), "Cannon_Hit");

	//当たり判定の形状を設定
	collisionShapeKind_ = CollisionShapeKind::Sphere;
	//当たり判定の半径を設定
	collisionRadius_ = 1.0f;

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/EnemyCannon");

	//影の初期化
	circleShadow_->worldTransform.scale = { 1.0f,1.0f,1.0f };
}

void EnemyCannon::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//死亡演出が終わったらアイドル状態にする
	if (state_ == State::kAsphyxia && !particle_->GetIsPlay()) {
		SetState(State::kIdle);
	}

	//弾がアクティブでないなら更新しない
	if (GetState() != State::kActive)
		return;

	//移動処理
	Move();
}

void EnemyCannon::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

	ImGui::Begin("敵キャノン");
	ImGui::DragFloat3("座標", &object3d_->worldTransform.translate.x, 0.01f);
	ImGui::End();

	//デバッグ用ラインのカラー
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

#endif // _DEBUG
}

void EnemyCannon::Spawn(const Vector3& _initPos, const Vector3& _targetPos) {
	//初期位置を保存
	object3d_->worldTransform.translate = _initPos;
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
		SetCollisionAttribute(CollisionAttribute::EnemyCannon);
		//アクティブ状態にする
		SetState(State::kActive);
		prePosition_ = { FLT_MAX,FLT_MAX ,FLT_MAX };

		return;
	}

	gravity_ = 2.0f * (maxHeight - _targetPos.y) / std::powf((hitTime / 2.0f), 2);
	//y方向の上昇速度を算出
	velocity_.y = 4.0f * (_initPos.y - _targetPos.y) / hitTime;

	//当たり判定属性をセット
	SetCollisionAttribute(CollisionAttribute::EnemyCannon);
	//アクティブ状態にする
	SetState(State::kActive);
	//前フレーム座標を初期化
	prePosition_ = { FLT_MAX,FLT_MAX ,FLT_MAX };
}


void EnemyCannon::OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) {
	//衝突時の共通処理ラムダ式
	auto commonCollisionProcess = [this, &subjectPos]() {
		//デバッグ用ラインのカラーを赤にする
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//パーティクルの発生
		TransformEuler transform = particle_->GetBaseTransform();
		transform.translate = object3d_->worldTransform.translate;
		particle_->SetBaseTransform(transform);
		particle_->SetIsPlay(true);
		//仮死状態にする
		SetState(State::kAsphyxia);
		};

	//当たり判定時の処理
	switch (attribute) {
		//プレイヤーに当たった場合
	case CollisionAttribute::Player:
		//共通処理
		commonCollisionProcess();

		//被弾インジケーターをつける
		playerUI_->GetHitIndicator()->RegistIndicator(generatedPosition_);

		break;
		//プレイヤー弾に当たった場合
	case CollisionAttribute::PlayerBullet:
		//共通処理
		commonCollisionProcess();

		//当たり判定属性をなしに
		SetCollisionAttribute(CollisionAttribute::Nothingness);

		break;
		//プレイヤーキャノンに当たった場合
	case CollisionAttribute::PlayerCannon:
		//共通処理
		commonCollisionProcess();

		//当たり判定属性をなしに
		SetCollisionAttribute(CollisionAttribute::Nothingness);

		break;
	default:
		break;
	}
}

void EnemyCannon::Move() {
	//重力をかける
	velocity_.y -= gravity_ * kDeltaTime;

	object3d_->worldTransform.translate += velocity_ * kDeltaTime;

	//弾が地面に当たったら
	if (object3d_->worldTransform.translate.y < 0.0f) {
		object3d_->worldTransform.translate.y = 0.0f;
		//アイドル状態にする
		SetState(State::kIdle);
	}
}
