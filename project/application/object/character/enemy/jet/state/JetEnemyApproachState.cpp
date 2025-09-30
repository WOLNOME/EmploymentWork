#include "JetEnemyApproachState.h"

//アプリケーション
#include <application/object/character/enemy/jet/base/IBaseJetEnemy.h>
#include <application/object/character/player/Player.h>

void JetEnemyApproachState::Enter(IBaseJetEnemy* enemy) {
}

void JetEnemyApproachState::Update(IBaseJetEnemy* enemy) {
	//死亡状態に切り替え
	TransitionDeadState(enemy);
	//攻撃範囲に入ったら攻撃状態に切り替え
	float searchPlayerDistanceAttack = enemy->GetParam()["searchPlayerDistanceAttack"];
	if (Vector3(enemy->GetPlayer()->GetWorldPosition() - enemy->GetWorldPosition()).Length() < searchPlayerDistanceAttack) {
		enemy->ChangeState("Attack");
	}
	//接近範囲から外れたらパトロール状態に切り替え
	float searchPlayerDistanceApproach = enemy->GetParam()["searchPlayerDistanceApproach"];
	if (Vector3(enemy->GetPlayer()->GetWorldPosition() - enemy->GetWorldPosition()).Length() > searchPlayerDistanceApproach) {
		enemy->ChangeState("Patrol");
	}
	//接近の更新処理
	UpdateApproach(enemy);
	//回転の更新処理
	UpdateRotate(enemy);
}

void JetEnemyApproachState::Exit(IBaseJetEnemy* enemy) {
}

void JetEnemyApproachState::UpdateApproach(IBaseJetEnemy* enemy) {
}
