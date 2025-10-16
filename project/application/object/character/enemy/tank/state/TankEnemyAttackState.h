#pragma once
#include "ITankEnemyState.h"

/// <summary>
/// 戦車型エネミーの攻撃状態を管理するクラス
/// </summary>
class TankEnemyAttackState : public ITankEnemyState {
public:
	TankEnemyAttackState(bool _isUseCannon);

	void Enter(IBaseTankEnemy* enemy) override;
	void Update(IBaseTankEnemy* enemy) override;
	void Exit(IBaseTankEnemy* enemy) override;

public://getter
	bool GetIsCannonFire() const { return isCannonFire_; }
public://setter
	void SetIsCannonFire(bool isCannonFire) { isCannonFire_ = isCannonFire; }

private:
	void UpdateAttack(IBaseTankEnemy* enemy);

private:
	//砲台を使うかどうか(砲台を使わない敵を作る過程で必要)
	bool isUseCannon_ = false;
	//砲弾攻撃変数
	float cannonCoolTimer_;
	bool isCannonFire_ = false;
};

