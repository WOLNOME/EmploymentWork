#pragma once
#include "IJetEnemyState.h"
class JetEnemyAttackState : public IJetEnemyState {
public:
	JetEnemyAttackState();

	void Enter(IBaseJetEnemy* enemy) override;
	void Update(IBaseJetEnemy* enemy) override;
	void Exit(IBaseJetEnemy* enemy) override;

private:
	void UpdateAttack(IBaseJetEnemy* enemy);
private:
	//爆弾攻撃変数
	float bombCoolTimer_;
	bool isBombFire_ = false;


};

