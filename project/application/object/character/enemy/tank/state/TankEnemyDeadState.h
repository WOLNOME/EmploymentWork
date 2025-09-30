#pragma once
#include "ITankEnemyState.h"

#include <Particle.h>
#include <memory>

class TankEnemyDeadState : public ITankEnemyState {
public:
	TankEnemyDeadState();

	void Enter(IBaseTankEnemy* enemy) override;
	void Update(IBaseTankEnemy* enemy) override;
	void Exit(IBaseTankEnemy* enemy) override;

private:
	std::unique_ptr<Particle> particle_ = nullptr;
	float particleOnTime_ = 0.5f;
	float particleOnTimer_ = 0.0f;
};

