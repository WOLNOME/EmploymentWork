#pragma once
#include "ITankEnemyState.h"
#include <Vector3.h>

/// <summary>
/// 戦車型エネミーのパトロール状態を管理するクラス
/// </summary>
class TankEnemyPatrolState : public ITankEnemyState {
public:
	void Enter(IBaseTankEnemy* enemy) override;
	void Update(IBaseTankEnemy* enemy) override;
	void Exit(IBaseTankEnemy* enemy) override;

private:
	void UpdatePatrol(IBaseTankEnemy* enemy);

private:
	//目標ポイント
	Vector3 targetPosition_ = {};
	//回転中か
	bool isRotation_ = false;
	//移動中か
	bool isMoving_ = false;
};

