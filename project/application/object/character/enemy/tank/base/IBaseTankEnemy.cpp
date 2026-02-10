#include "IBaseTankEnemy.h"
#include "CombinedParticleManager.h"
#include <TextureManager.h>
#include <ImGuiManager.h>
#include <ParticleManager.h>
#include <cassert>

//アプリケーション
#include "application/object/character/player/Player.h"
#include "application/object/character/item/manager/ItemManager.h"
#include <application/object/character/enemy/tank/collision/TankCollider.h>

using namespace Norm;

IBaseTankEnemy::IBaseTankEnemy() {
	//状態管理用変数の初期化
	patrolState_ = std::make_unique<TankEnemyPatrolState>();
	approachState_ = std::make_unique <TankEnemyApproachState>();
	attackState_ = std::make_unique <TankEnemyAttackState>();
	deadState_ = std::make_unique <TankEnemyDeadState>();
	//移動パーティクルの生成・初期化
	moveParticle_ = std::make_unique<CombinedParticle>();
	moveParticle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("TankEnemyMove"), "Tank_Move");
	moveParticle_->SetIsRepeat(true);
	//初期ステートを決定
	currentStateName_ = StateName::kPatrol;
	currentState_ = patrolState_.get();
}

void IBaseTankEnemy::Initialize() {
	//ベースキャラクターの初期化
	BaseCharacter::Initialize();

	//当たり判定の生成・初期化
	collider_ = std::make_unique<TankCollider>(this);
	collider_->SetCollisionAttribute(CollisionAttribute::Nothingness);

}

void IBaseTankEnemy::Update() {
	//ベースキャラクターの更新
	BaseCharacter::Update();

	//現在ステートの更新
	currentState_->Update(this);

	//移動パーティクルの座標を合わせる
	TransformEuler particleTransform = moveParticle_->GetBaseTransform();
	particleTransform.translate = object3d_->worldTransform.GetTranslate();
	moveParticle_->SetBaseTransform(particleTransform);
}

void IBaseTankEnemy::DebugWithImGui() {
#ifdef _DEBUG
	//ベースキャラクターのデバッグ処理
	BaseCharacter::DebugWithImGui();
#endif // _DEBUG
}

void IBaseTankEnemy::ChangeState(const std::string& stateName) {
	//新しい状態を決める
	ITankEnemyState* newState = nullptr;
	if (stateName == "Patrol") {
		newState = patrolState_.get();
		currentStateName_ = StateName::kPatrol;
		//移動パーティクルをオンにする
		moveParticle_->SetIsPlay(true);
	}
	else if (stateName == "Approach") {
		newState = approachState_.get();
		currentStateName_ = StateName::kApproach;
		//移動パーティクルをオンにする
		moveParticle_->SetIsPlay(true);
	}
	else if (stateName == "Attack") {
		newState = attackState_.get();
		currentStateName_ = StateName::kAttack;
		//移動パーティクルをオフにする
		moveParticle_->SetIsPlay(false);
	}
	else if (stateName == "Dead") {
		newState = deadState_.get();
		currentStateName_ = StateName::kDead;
		//移動パーティクルをオフにする
		moveParticle_->SetIsPlay(false);

		//キャノ太なら
		if (param_["tag"] == "canota") {
			//回復アイテムを生成
			itemManager_->SpawnHealItem(GetWorldTransform().GetTranslate());
		}
		//キーキャノ太なら
		if (param_["tag"] == "keyCanota") {
			//キーアイテムを生成
			itemManager_->SpawnKeyItem(GetWorldTransform().GetTranslate());
		}
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

void IBaseTankEnemy::SetEnemyUI(EnemyUI* _enemyUI) {
	//インスタンスをセット
	enemyUI_ = _enemyUI;

	patrolState_->SetEnemyUI(enemyUI_);
	approachState_->SetEnemyUI(enemyUI_);
}