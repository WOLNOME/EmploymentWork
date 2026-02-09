#pragma once
#include "ITankEnemyState.h"
#include <Vector3.h>

//前方宣言
class EnemyUI;

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

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// 敵UIのセット
	/// </summary>
	/// <param name="_enemyUI">敵UI</param>
	void SetEnemyUI(EnemyUI* _enemyUI) { enemyUI_ = _enemyUI; }

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
	///		インスタンス
	/// ============================== ///

	EnemyUI* enemyUI_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//目標ポイント
	Norm::Vector3 targetPosition_ = {};
	//回転中か
	bool isRotation_ = false;
	//移動中か
	bool isMoving_ = false;
};

