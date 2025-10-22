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
	/// <param name="_isUseCannon">砲弾使用フラグ</param>
	TankEnemyAttackState(bool _isUseCannon);

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
	///		getter
	/// ============================== ///

	/// <summary>
	/// 砲弾発射フラグの取得
	/// </summary>
	/// <returns>砲弾発射フラグ</returns>
	bool GetIsCannonFire() const { return isCannonFire_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// 砲弾発射フラグの設定
	/// </summary>
	/// <param name="isCannonFire">砲弾発射フラグ</param>
	void SetIsCannonFire(bool isCannonFire) { isCannonFire_ = isCannonFire; }

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

	//砲台を使うかどうか(砲台を使わない敵を作る過程で必要)
	bool isUseCannon_ = false;
	//砲弾攻撃変数
	float cannonCoolTimer_;
	bool isCannonFire_ = false;
};

