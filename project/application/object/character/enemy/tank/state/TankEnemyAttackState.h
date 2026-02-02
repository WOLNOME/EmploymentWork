#pragma once
#include "ITankEnemyState.h"

/// <summary>
/// 戦車型エネミーの攻撃状態を管理するクラス
/// </summary>
class TankEnemyAttackState : public ITankEnemyState {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TankEnemyAttackState();

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
	/// 攻撃処理の更新
	/// </summary>
	/// <param name="enemy">タンクエネミーのポインタ</param>
	void UpdateAttack(IBaseTankEnemy* enemy);

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//攻撃のクールタイマー
	float coolTimer_ = 0.0f;
};

