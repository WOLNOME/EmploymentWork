#pragma once
#include "ITankEnemyState.h"

#include <CombinedParticle.h>
#include <memory>

/// <summary>
/// 戦車型エネミーの死亡状態を管理するクラス
/// </summary>
class TankEnemyDeadState : public ITankEnemyState {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TankEnemyDeadState();

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
	///		メンバ関数
	/// ============================== ///

	std::unique_ptr<Norm::CombinedParticle> particle_ = nullptr;
};

