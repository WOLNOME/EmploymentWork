#include "JetEnemyAttackState.h"

//アプリケーション
#include <application/object/character/enemy/jet/base/IBaseJetEnemy.h>
#include <application/object/character/player/Player.h>

JetEnemyAttackState::JetEnemyAttackState() {
}

void JetEnemyAttackState::Enter(IBaseJetEnemy* enemy) {
	isCanBombFire_ = true;
	phaseChangeCoolTimer_ = 0.0f;
}

void JetEnemyAttackState::Update(IBaseJetEnemy* enemy) {
	//死亡状態に切り替え
	TransitionDeadState(enemy);

	//攻撃済みなら接近状態に切り替え
	if (!isCanBombFire_) {
		phaseChangeCoolTimer_ += kDeltaTime;
		if (phaseChangeCoolTimer_ > kPhaseChangeCoolTime_) {
			enemy->ChangeState("Approach");
		}
	}

	//移動の更新処理(既存の速度を参照して直進)
	Vector3 currentPosition = enemy->GetWorldTransform().translate;
	Vector3 velocity = enemy->GetVelocity();
	currentPosition += velocity * kDeltaTime;
	enemy->SetTranslate(currentPosition);

}

void JetEnemyAttackState::Exit(IBaseJetEnemy* enemy) {
}
