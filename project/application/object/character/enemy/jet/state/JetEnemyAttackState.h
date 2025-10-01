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
	bool GetIsCanBombFire() const { return isCanBombFire_; }
	//setter
	void SetIsCanBombFire(bool isBombFire) { isCanBombFire_ = isBombFire; }

private:
	//爆弾投下可能かどうか
	bool isCanBombFire_ = false;

	//フェーズ移行クールタイム
	const float phaseChangeCoolTime_ = 2.0f;
	float phaseChangeCoolTimer_ = 0.0f;
};

