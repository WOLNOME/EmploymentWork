#include "EnemyAttackState.h"

//アプリケーション
#include <application/object/character/enemy/base/IBaseEnemy.h>
#include <application/object/character/player/Player.h>

void EnemyAttackState::Enter(IBaseEnemy* enemy) {
}

void EnemyAttackState::Update(IBaseEnemy* enemy) {
	//死亡状態に切り替え
	TransitionDeadState(enemy);

	//攻撃範囲から外れたら接近状態に切り替え
	float searchPlayerDistanceAttack = enemy->GetParam()["searchPlayerDistanceAttack"];
	if (Vector3(enemy->GetPlayer()->GetWorldPosition() - enemy->GetWorldPosition()).Length() > searchPlayerDistanceAttack) {
		enemy->ChangeState("Approach");
	}

	//回転の更新処理
	UpdateRotate(enemy);

}

void EnemyAttackState::Exit(IBaseEnemy* enemy) {
}
