#pragma once
#include "ITankEnemyState.h"
#include <Vector3.h>

/// <summary>
/// 戦車型エネミーのパトロール状態を管理するクラス
/// </summary>
class TankEnemyPatrolState : public ITankEnemyState {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// 切り替え時の最初の処理
	/// </summary>
	/// <param name="enemy">タンクエネミーのポインタ</param>
	void Enter(IBaseTankEnemy* enemy) override;
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="enemy">タンクエネミーのポインタ</param>
	void Update(IBaseTankEnemy* enemy) override;
	/// <summary>
	/// 切り替え時の最後の処理
	/// </summary>
	/// <param name="enemy">タンクエネミーのポインタ</param>
	void Exit(IBaseTankEnemy* enemy) override;

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// パトロールの更新処理
	/// </summary>
	/// <param name="enemy">タンクエネミーのポインタ</param>
	void UpdatePatrol(IBaseTankEnemy* enemy);

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//目標ポイント
	Vector3 targetPosition_ = {};
	//回転中か
	bool isRotation_ = false;
	//移動中か
	bool isMoving_ = false;
};

