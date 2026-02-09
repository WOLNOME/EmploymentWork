#pragma once
#include "ITankEnemyState.h"

//前方宣言
class EnemyUI;

/// <summary>
/// 戦車型エネミーの接近状態を管理するクラス
/// </summary>
class TankEnemyApproachState : public ITankEnemyState {
public:
	/// ============================== ///
	///		メンバ関数（public）
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
	/// 敵UIをセット
	/// </summary>
	/// <param name="_enemyUI">敵UI</param>
	void SetEnemyUI(EnemyUI* _enemyUI) { enemyUI_ = _enemyUI; }

private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	EnemyUI* enemyUI_ = nullptr;

	/// ============================== ///
	///		メンバ関数（private）
	/// ============================== ///

	/// <summary>
	/// 接近の更新処理
	/// </summary>
	/// <param name="enemy">タンクエネミーのポインタ</param>
	void UpdateApproach(IBaseTankEnemy* enemy);
};

