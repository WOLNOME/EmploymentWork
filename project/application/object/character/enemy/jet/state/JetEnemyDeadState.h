#pragma once
#include "IJetEnemyState.h"

#include <Particle.h>
#include <memory>

/// <summary>
/// ジェットエネミーの死亡状態を管理するクラス
/// </summary>
class JetEnemyDeadState : public IJetEnemyState {
public:
	JetEnemyDeadState();
	
	void Enter(IBaseJetEnemy* enemy) override;
	void Update(IBaseJetEnemy* enemy) override;
	void Exit(IBaseJetEnemy* enemy) override;

private:
	std::unique_ptr<Particle> particle_ = nullptr;
	float particleOnTime_ = 0.5f;
	float particleOnTimer_ = 0.0f;
};

