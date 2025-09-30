#pragma once
#include "ITankEnemyState.h"
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
	bool isUseCannon_ = false;
	//砲弾攻撃変数
	float cannonCoolTimer_;
	bool isCannonFire_ = false;
};

