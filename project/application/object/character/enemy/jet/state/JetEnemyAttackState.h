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

public:
	/// ============================== ///
	///		getter
	/// ============================== ///
	
	/// <summary>
	/// 爆弾投下可能かどうかを取得する
	/// </summary>
	/// <returns>爆弾投下可能かどうか</returns>
	bool GetIsCanBombFire() const { return isCanBombFire_; }

	/// ============================== ///
	///		setter
	/// ============================== ///
	
	/// <summary>
	/// 爆弾投下可能かどうかを設定する
	/// </summary>
	/// <param name="isBombFire">爆弾投下可能かどうか</param>
	void SetIsCanBombFire(bool isBombFire) { isCanBombFire_ = isBombFire; }


private:
	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//爆弾投下可能かどうか
	bool isCanBombFire_ = false;

	//フェーズ移行クールタイム
	const float kPhaseChangeCoolTime_ = 2.0f;
	float phaseChangeCoolTimer_ = 0.0f;
};

