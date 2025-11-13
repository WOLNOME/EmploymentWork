#include "IBaseTankEnemy.h"
#include <TextureManager.h>
#include <ImGuiManager.h>
#include <ParticleManager.h>
#include <cassert>

//アプリケーション
#include "application/object/character/player/Player.h"
#include "application/object/character/item/manager/ItemManager.h"

IBaseTankEnemy::IBaseTankEnemy(bool _isUseCannon) {
	//状態管理用変数の初期化
	patrolState_ = std::make_unique<TankEnemyPatrolState>();
	approachState_ = std::make_unique <TankEnemyApproachState>();
	attackState_ = std::make_unique <TankEnemyAttackState>(_isUseCannon);
	deadState_ = std::make_unique <TankEnemyDeadState>();
	//初期ステートを決定
	currentState_ = patrolState_.get();
}

void IBaseTankEnemy::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//当たり判定の形状を設定
	collisionShapeKind_ = CollisionShapeKind::OBB;
	//当たり判定の属性を設定
	SetCollisionAttribute(CollisionAttribute::Enemy);
}

void IBaseTankEnemy::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//現在ステートの更新
	currentState_->Update(this);
}

void IBaseTankEnemy::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

	//デバッグ用ラインのカラー
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };

#endif // _DEBUG
}

void IBaseTankEnemy::OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) {
	//当たり判定時の処理
	switch (attribute) {
		//プレイヤーに当たった場合
	case CollisionAttribute::Player: {
		//HPを減らす
		hp_ -= 1;
		//0~MaxHPの範囲に収める
		hp_ = std::clamp(hp_, 0, maxHP_);

		//相手の座標の方向と反対方向のベクトルを速度に加算
		Vector3 reflectVec = -(subjectPos - GetWorldTransform().translate).Normalized();
		velocity_.x += reflectVec.x * 50.0f;
		velocity_.z += reflectVec.z * 50.0f;

		break;
	}
								   //エネミーに当たった場合
	case CollisionAttribute::Enemy: {
		//相手の座標と反対方向のベクトルを速度に加算
		Vector3 reflectVec = -(subjectPos - GetWorldTransform().translate).Normalized();
		velocity_.x += reflectVec.x * 30.0f;
		velocity_.z += reflectVec.z * 30.0f;

		break;
	}
								  //プレイヤーキャノンに当たった場合
	case CollisionAttribute::PlayerCannon:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//HPを減らす
		hp_ -= 10;
		//0~MaxHPの範囲に収める
		hp_ = std::clamp(hp_, 0, maxHP_);

		break;
		//プレイヤー弾に当たった場合
	case CollisionAttribute::PlayerBullet:
		debugLineColor_ = { 1.0f,0.0f,0.0f,1.0f };
		//HPを減らす
		hp_ -= 1;
		//0~MaxHPの範囲に収める
		hp_ = std::clamp(hp_, 0, maxHP_);

		break;
	default:
		break;
	}
}

void IBaseTankEnemy::ChangeState(const std::string& stateName) {
	//新しい状態を決める
	ITankEnemyState* newState = nullptr;
	if (stateName == "Patrol") {
		newState = patrolState_.get();
	}
	else if (stateName == "Approach") {
		newState = approachState_.get();
	}
	else if (stateName == "Attack") {
		newState = attackState_.get();
	}
	else if (stateName == "Dead") {
		newState = deadState_.get();
		//アイテムを生成
		itemManager_->AddItem(GetWorldTransform().translate);
	}
	else {
		assert(0 && "使用できない名前が使われています。");
	}
	//現在の状態を終了する
	if (currentState_) {
		currentState_->Exit(this);
	}
	//新しい状態を開始する
	currentState_ = newState;
	if (currentState_) {
		currentState_->Enter(this);
	}
}
