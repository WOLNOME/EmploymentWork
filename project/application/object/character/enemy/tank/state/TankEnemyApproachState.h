#pragma once
#include "ITankEnemyState.h"

/// <summary>
/// 戦車型エネミーの接近状態を管理するクラス
/// </summary>
class TankEnemyApproachState : public ITankEnemyState {
public:
	void Enter(IBaseTankEnemy* enemy) override;
	void Update(IBaseTankEnemy* enemy) override;
	void Exit(IBaseTankEnemy* enemy) override;

private:
	void UpdateApproach(IBaseTankEnemy* enemy);
};

