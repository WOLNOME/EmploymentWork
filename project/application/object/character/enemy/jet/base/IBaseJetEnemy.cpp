#include "IBaseJetEnemy.h"
#include <Object3dManager.h>

//アプリケーション
#include <application/object/character/enemy/jet/collision/JetCollider.h>


using namespace Norm;

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
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/jet");

	//オブジェクトの生成と初期化
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("Jet"), "jet");
	object3d_->SetIsDisplay(false);
	//ワールドトランスフォームの初期化
	worldTransform_.SetTranslate({ FLT_MAX,FLT_MAX ,FLT_MAX });

	//当たり判定の初期化
	auto* jetCollider = dynamic_cast<JetCollider*>(collider_.get());
	collider_ = std::make_unique<JetCollider>(this);
	collider_->SetCollisionAttribute(CollisionAttribute::Nothingness);
	collider_->SetWorldTransform(&worldTransform_);
	collider_->SetOffset({
		param_["collisionCenterOffsetOBB"]["x"],
		param_["collisionCenterOffsetOBB"]["y"],
		param_["collisionCenterOffsetOBB"]["z"]
		});
	jetCollider->SetOBBSize({
		param_["collisionSizeOBB"]["x"],
		param_["collisionSizeOBB"]["y"],
		param_["collisionSizeOBB"]["z"]
		});

	//パラメータの反映
	maxHP_ = param_["maxHP"];
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
#endif // _DEBUG
}

void IBaseJetEnemy::SetMessageUI(MessageUI* messageUI) {
	patrolState_->SetMessageUI(messageUI);
	approachState_->SetMessageUI(messageUI);
}

void IBaseJetEnemy::SetEnemyUI(EnemyUI* _enemyUI) {
	//インスタンスのセット
	enemyUI_ = _enemyUI;

	patrolState_->SetEnemyUI(enemyUI_);
	approachState_->SetEnemyUI(enemyUI_);
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
	//現在の状態の終了処理
	if (currentState_) {
		currentState_->Exit(this);
	}
	//新しい状態の開始処理
	currentState_ = newState;
	if (currentState_) {
		currentState_->Enter(this);
	}
}
