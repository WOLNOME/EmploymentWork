#include "JetEnemyAttackState.h"

//アプリケーション
#include <application/object/character/enemy/jet/base/IBaseJetEnemy.h>
#include <application/object/character/player/Player.h>

JetEnemyAttackState::JetEnemyAttackState() {
}

void JetEnemyAttackState::Enter(IBaseJetEnemy* enemy) {
}

void JetEnemyAttackState::Update(IBaseJetEnemy* enemy) {
	//死亡状態に切り替え
	TransitionDeadState(enemy);

	//攻撃範囲から外れたら接近状態に切り替え
	float searchPlayerDistanceAttack = enemy->GetParam()["searchPlayerDistanceAttack"];
	if (Vector3(enemy->GetPlayer()->GetWorldPosition() - enemy->GetWorldPosition()).Length() > searchPlayerDistanceAttack) {
		enemy->ChangeState("Approach");
	}

	//回転の更新処理
	UpdateRotate(enemy);
	//攻撃の更新処理
	UpdateAttack(enemy);
}

void JetEnemyAttackState::Exit(IBaseJetEnemy* enemy) {
}

void JetEnemyAttackState::UpdateAttack(IBaseJetEnemy* enemy) {
	//クールタイム処理
	if (bombCoolTimer_ > 0.0f) {
		bombCoolTimer_ -= kDeltaTime;
		//クールタイムがマイナスになったら0にする
		if (bombCoolTimer_ < 0.0f) {
			bombCoolTimer_ = 0.0f;
		}
		//クールタイム処理を終えたら関数を抜ける
		return;
	}

	//未攻撃状態なら攻撃処理
	if (!isBombFire_) {
		//砲弾を発射したフラグをオン
		isBombFire_ = true;
		//クールタイムをセット
		float bombCoolTime = enemy->GetParam()["bombCoolTime"];
		bombCoolTimer_ = bombCoolTime;
	}
}
