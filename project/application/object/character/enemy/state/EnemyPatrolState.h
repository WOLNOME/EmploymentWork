#pragma once
#include "IEnemyState.h"
#include <Vector3.h>

class EnemyPatrolState : public IEnemyState {
public:
	void Enter(IBaseEnemy* enemy) override;
	void Update(IBaseEnemy* enemy) override;
	void Exit(IBaseEnemy* enemy) override;

private:
	void UpdatePatrol(IBaseEnemy* enemy);

private:
	//目標ポイント
	Vector3 targetPosition_ = {};
	//回転中か
	bool isRotation_ = false;
	//移動中か
	bool isMoving_ = false;
};

