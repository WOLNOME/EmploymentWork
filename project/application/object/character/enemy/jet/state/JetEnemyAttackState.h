#pragma once
#include "IJetEnemyState.h"

/// <summary>
/// ジェットエネミーの攻撃状態を管理するクラス
/// </summary>
class JetEnemyAttackState : public IJetEnemyState {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	JetEnemyAttackState();

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
	///		メンバ関数（private）
	/// ============================== ///

	/// <summary>
	/// 移動の更新処理
	/// </summary>
	/// <param name="enemy">ジェットエネミーのポインタ</param>
	void UpdateMove(IBaseJetEnemy* enemy);

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//フェーズ移行クールタイム
	const float kCoolTime_ = 2.0f;
	float coolTimer_ = 0.0f;
};

