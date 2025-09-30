#pragma once
#include "IJetEnemyState.h"
#include <Vector3.h>

class JetEnemyPatrolState : public IJetEnemyState {
public:
	void Enter(IBaseJetEnemy* enemy) override;
	void Update(IBaseJetEnemy* enemy) override;
	void Exit(IBaseJetEnemy* enemy) override;

private:
	void UpdatePatrol(IBaseJetEnemy* enemy);

private:
	//目標ポイント
	Vector3 targetPosition_ = {};
	//回転中か
	bool isRotation_ = false;
	//移動中か
	bool isMoving_ = false;
};

