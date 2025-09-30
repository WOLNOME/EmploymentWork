#pragma once
#include "ITankEnemyState.h"
class TankEnemyApproachState : public ITankEnemyState {
public:
	void Enter(IBaseTankEnemy* enemy) override;
	void Update(IBaseTankEnemy* enemy) override;
	void Exit(IBaseTankEnemy* enemy) override;

private:
	void UpdateApproach(IBaseTankEnemy* enemy);
};

