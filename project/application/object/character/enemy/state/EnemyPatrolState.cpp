#include "EnemyPatrolState.h"
#include <Vector3.h>

//アプリケーション
#include <application/object/character/enemy/base/IBaseEnemy.h>
#include <application/object/character/player/Player.h>

void EnemyPatrolState::Enter(IBaseEnemy* enemy) {
}

void EnemyPatrolState::Update(IBaseEnemy* enemy) {
	//プレイヤーが近づいたら接近状態に切り替え
	float searchPlayerDistanceApproach = enemy->GetParam()["searchPlayerDistanceApproach"];
	if (Vector3(enemy->GetPlayer()->GetWorldPosition() - enemy->GetWorldPosition()).Length() < searchPlayerDistanceApproach) {
		enemy->ChangeState(enemy->GetApproachState());
	}
}

void EnemyPatrolState::Exit(IBaseEnemy* enemy) {

}
