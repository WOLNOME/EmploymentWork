#pragma once
#include "IJetEnemyState.h"
#include <Vector3.h>

class MessageUI;
class JetEnemyApproachState : public IJetEnemyState {
public:
	void Enter(IBaseJetEnemy* enemy) override;
	void Update(IBaseJetEnemy* enemy) override;
	void Exit(IBaseJetEnemy* enemy) override;

	//setter
	void SetMessageUI(MessageUI* messageUI) { messageUI_ = messageUI; }

private:
	void UpdateApproach(IBaseJetEnemy* enemy);

private:
	MessageUI* messageUI_ = nullptr;

	//目標点
	Vector3 targetPosition_ = {0.0f,0.0f,0.0f};

	//状態継続最低時間
	const float stateContinueTime_ = 2.0f;
	float stateContinueTimer_ = 0.0f;

};

