#pragma once
#include "IJetEnemyState.h"
#include <Vector3.h>
#include <string>

class MessageUI;
class JetEnemyPatrolState : public IJetEnemyState {
public:
	void Enter(IBaseJetEnemy* enemy) override;
	void Update(IBaseJetEnemy* enemy) override;
	void Exit(IBaseJetEnemy* enemy) override;

	//setter
	void SetMessageUI(MessageUI* messageUI) { messageUI_ = messageUI; }

private:
	void UpdatePatrol(IBaseJetEnemy* enemy);

private:
	//メッセージUI
	MessageUI* messageUI_ = nullptr;

	//目標ポイント
	Vector3 targetPosition_ = {};

	//状態継続最低時間
	const float stateContinueTime_ = 2.0f;
	float stateContinueTimer_ = 0.0f;

};

