#include "IBaseJetEnemy.h"

IBaseJetEnemy::IBaseJetEnemy() {
	//状態管理用変数の初期化
	patrolState_ = std::make_unique<JetEnemyPatrolState>();
	approachState_ = std::make_unique <JetEnemyApproachState>();
	attackState_ = std::make_unique <JetEnemyAttackState>();
	deadState_ = std::make_unique <JetEnemyDeadState>();
	//初期ステートを決定
	currentState_ = patrolState_.get();
}

void IBaseJetEnemy::Initialize() {
	//当たり判定の形状を設定
	collisionShapeKind_ = CollisionShapeKind::OBB;
	//当たり判定の属性を設定
	SetCollisionAttribute(CollisionAttribute::Enemy);
}

void IBaseJetEnemy::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//現在ステートの更新
	currentState_->Update(this);
}

void IBaseJetEnemy::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();

	//デバッグ用ラインのカラー
	debugLineColor_ = { 1.0f,1.0f,1.0f,1.0f };
#endif // _DEBUG
}

void IBaseJetEnemy::SetMessageUI(MessageUI* messageUI) {
	patrolState_->SetMessageUI(messageUI);
	approachState_->SetMessageUI(messageUI);
}

void IBaseJetEnemy::OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) {
	//当たり判定時の処理
	switch (attribute) {
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

void IBaseJetEnemy::ChangeState(const std::string& stateName) {
	//新しい状態を決める
	IJetEnemyState* newState = nullptr;
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
	}
	else {
		assert(0 && "使用できない名前が使われています。");
	}

	if (currentState_) {
		currentState_->Exit(this);
	}
	currentState_ = newState;
	if (currentState_) {
		currentState_->Enter(this);
	}
}
