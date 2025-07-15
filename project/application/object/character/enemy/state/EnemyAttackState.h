#pragma once
#include "IEnemyState.h"
class EnemyAttackState : public IEnemyState {
public:
	EnemyAttackState(bool _isUseCannon);

	void Enter(IBaseEnemy* enemy) override;
	void Update(IBaseEnemy* enemy) override;
	void Exit(IBaseEnemy* enemy) override;

public://getter
	bool GetIsCannonFire() const { return isCannonFire_; }

private:
	void UpdateAttack(IBaseEnemy* enemy);

private:
	bool isUseCannon_ = false;
	//砲弾攻撃変数
	float cannonCoolTimer_;
	bool isCannonFire_ = false;
};

