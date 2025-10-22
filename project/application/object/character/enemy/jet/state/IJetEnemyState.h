#pragma once

class IBaseJetEnemy;

/// <summary>
/// ジェットエネミーの状態を表すインターフェースクラス
/// 状態の遷移や更新処理を定義する
/// </summary>
class IJetEnemyState {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IJetEnemyState() = default;
	/// <summary>
	/// 切り替え時の最初の処理
	/// </summary>
	/// <param name="enemy">ジェットエネミーのポインタ</param>
	virtual void Enter(IBaseJetEnemy* enemy) = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="enemy">ジェットエネミーのポインタ</param>
	virtual void Update(IBaseJetEnemy* enemy) = 0;
	/// <summary>
	/// 切り替え時の最後の処理
	/// </summary>
	/// <param name="enemy">ジェットエネミーのポインタ</param>
	virtual void Exit(IBaseJetEnemy* enemy) = 0;

protected:
	/// ============================== ///
	///		公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// 死亡処理への遷移
	/// </summary>
	/// <param name="enemy">ジェットエネミーのポインタ</param>
	void TransitionDeadState(IBaseJetEnemy* enemy);
};

