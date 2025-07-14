#pragma once
#include "IEnemyState.h"

#include <Particle.h>
#include <memory>

class EnemyDeadState : public IEnemyState {
public:
	EnemyDeadState();

	void Enter(IBaseEnemy* enemy) override;
	void Update(IBaseEnemy* enemy) override;
	void Exit(IBaseEnemy* enemy) override;

private:
	std::unique_ptr<Particle> particle_ = nullptr;
	float particleOnTime_ = 0.5f;
	float particleOnTimer_ = 0.0f;
};

