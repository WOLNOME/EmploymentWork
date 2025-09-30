#include "JetEnemyPatrolState.h"
#include <Vector3.h>
#include <random>

//アプリケーション
#include <application/object/character/enemy/jet/base/IBaseJetEnemy.h>
#include <application/object/character/player/Player.h>

void JetEnemyPatrolState::Enter(IBaseJetEnemy* enemy) {
	isRotation_ = false;
	isMoving_ = false;
}

void JetEnemyPatrolState::Update(IBaseJetEnemy* enemy) {
	//死亡状態に切り替え
	TransitionDeadState(enemy);

	//プレイヤーが近づいたら接近状態に切り替え
	float searchPlayerDistanceApproach = enemy->GetParam()["searchPlayerDistanceApproach"];
	if (Vector3(enemy->GetPlayer()->GetWorldPosition() - enemy->GetWorldPosition()).Length() < searchPlayerDistanceApproach) {
		enemy->ChangeState("Approach");
	}

	//パトロールの更新処理
	UpdatePatrol(enemy);
}

void JetEnemyPatrolState::Exit(IBaseJetEnemy* enemy) {
}

void JetEnemyPatrolState::UpdatePatrol(IBaseJetEnemy* enemy) {
}
