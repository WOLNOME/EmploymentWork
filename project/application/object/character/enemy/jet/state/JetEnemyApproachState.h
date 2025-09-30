#pragma once
#include "IJetEnemyState.h"
class JetEnemyApproachState : public IJetEnemyState {
public:
	void Enter(IBaseJetEnemy* enemy) override;
	void Update(IBaseJetEnemy* enemy) override;
	void Exit(IBaseJetEnemy* enemy) override;

private:
	void UpdateApproach(IBaseJetEnemy* enemy);
};

