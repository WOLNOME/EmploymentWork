#pragma once
#include "IJetEnemyState.h"
class JetEnemyAttackState : public IJetEnemyState {
public:
	JetEnemyAttackState();

	void Enter(IBaseJetEnemy* enemy) override;
	void Update(IBaseJetEnemy* enemy) override;
	void Exit(IBaseJetEnemy* enemy) override;

public:
	//getter
	bool GetIsBombFire() const { return isBombFire_; }
	//setter
	void SetIsBombFire(bool isBombFire) { isBombFire_ = isBombFire; }

private:
	//爆弾投下済みかどうか
	bool isBombFire_ = false;

	//フェーズ移行クールタイム
	const float phaseChangeCoolTime_ = 2.0f;
	float phaseChangeCoolTimer_ = 0.0f;
};

