#include "JetEnemyAttackState.h"

//アプリケーション
#include <application/object/character/enemy/jet/base/IBaseJetEnemy.h>
#include <application/object/character/player/Player.h>
#include <application/object/character/weapon/enemy/manager/EnemyWeaponManager.h>

JetEnemyAttackState::JetEnemyAttackState() {
}

void JetEnemyAttackState::Enter(IBaseJetEnemy* enemy) {
	//クールタイマーの初期化
	coolTimer_ = 0.0f;
	//攻撃処理の通知
	Vector3 targetPos = enemy->GetWorldTransform().translate;
	targetPos.y = 0.0f;
	enemy->GetEnemyWeaponManager()->SpawnBomb(BombMethod::Fall, enemy->GetWorldTransform().translate, targetPos);
}

void JetEnemyAttackState::Update(IBaseJetEnemy* enemy) {
	//死亡状態に切り替え
	TransitionDeadState(enemy);

	//攻撃クールタイムの更新
	coolTimer_ += kDeltaTime;
	if (coolTimer_ > kCoolTime_) {
		enemy->ChangeState("Approach");
	}

	//移動の更新処理(既存の速度を参照して直進)
	Vector3 currentPosition = enemy->GetWorldTransform().translate;
	Vector3 velocity = enemy->GetVelocity();
	currentPosition += velocity * kDeltaTime;
	enemy->SetTranslate(currentPosition);

}

void JetEnemyAttackState::Exit(IBaseJetEnemy* enemy) {
}
