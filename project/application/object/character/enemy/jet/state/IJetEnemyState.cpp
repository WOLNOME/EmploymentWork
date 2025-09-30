#include "IJetEnemyState.h"

//アプリケーション
#include <application/object/character/enemy/jet/base/IBaseJetEnemy.h>
#include <application/object/character/player/Player.h>

void IJetEnemyState::TransitionDeadState(IBaseJetEnemy* enemy) {
	if (enemy->GetHP() <= 0) {
		enemy->ChangeState("Dead");
	}
}

void IJetEnemyState::UpdateRotate(IBaseJetEnemy* enemy) {
}
