#include "EnemyAttackState.h"

//アプリケーション
#include <application/object/character/enemy/base/IBaseEnemy.h>
#include <application/object/character/player/Player.h>

EnemyAttackState::EnemyAttackState(bool _isUseCannon) {
	isUseCannon_ = _isUseCannon;
}

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
	//攻撃の更新処理
	UpdateAttack(enemy);

}

void EnemyAttackState::Exit(IBaseEnemy* enemy) {
}

void EnemyAttackState::UpdateAttack(IBaseEnemy* enemy) {
	//クールタイム処理
	if (cannonCoolTimer_ > 0.0f) {
		cannonCoolTimer_ -= kDeltaTime;
		//クールタイムがマイナスになったら0にする
		if (cannonCoolTimer_ < 0.0f) {
			cannonCoolTimer_ = 0.0f;
		}
		//砲弾を発射したフラグをオフ
		isCannonFire_ = false;
		//クールタイム処理を終えたら関数を抜ける
		return;
	}

	//未攻撃状態なら攻撃処理
	if (!isCannonFire_ && isUseCannon_) {
		//砲弾を発射したフラグをオン
		isCannonFire_ = true;
		//クールタイムをセット
		float cannonCoolTime = enemy->GetParam()["cannonCoolTime"];
		cannonCoolTimer_ = cannonCoolTime;
	}
}
