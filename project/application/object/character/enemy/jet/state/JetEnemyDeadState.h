#pragma once
#include "IJetEnemyState.h"

#include <Particle.h>
#include <memory>

/// <summary>
/// ジェットエネミーの死亡状態を管理するクラス
/// </summary>
class JetEnemyDeadState : public IJetEnemyState {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	JetEnemyDeadState();
	
	/// <summary>
	/// 切り替え時の最初の処理
	/// </summary>
	/// <param name="enemy">ジェットエネミーのポインタ</param>
	void Enter(IBaseJetEnemy* enemy) override;
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="enemy">ジェットエネミーのポインタ</param>
	void Update(IBaseJetEnemy* enemy) override;
	/// <summary>
	/// 切り替え時の最後の処理
	/// </summary>
	/// <param name="enemy">ジェットエネミーのポインタ</param>
	void Exit(IBaseJetEnemy* enemy) override;

private:
	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	std::unique_ptr<Particle> particle_ = nullptr;
	float particleOnTime_ = 0.5f;
	float particleOnTimer_ = 0.0f;
};

