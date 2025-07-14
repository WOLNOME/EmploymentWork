#pragma once
#include "IEnemyState.h"
class EnemyApproachState : public IEnemyState {
public:
	void Enter(IBaseEnemy* enemy) override;
	void Update(IBaseEnemy* enemy) override;
	void Exit(IBaseEnemy* enemy) override;

private:
	void UpdateApproach(IBaseEnemy* enemy);
};

