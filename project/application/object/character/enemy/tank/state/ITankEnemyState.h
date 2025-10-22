#pragma once

class IBaseTankEnemy;

/// <summary>
/// 戦車型敵キャラクターの状態を表すインターフェースクラス
/// 状態の遷移や更新処理を定義する
/// </summary>
class ITankEnemyState {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ITankEnemyState() = default;
	/// <summary>
	/// 切り替え時の最初の処理
	/// </summary>
	/// <param name="enemy">タンクエネミーのポインタ</param>
	virtual void Enter(IBaseTankEnemy* enemy) = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="enemy">タンクエネミーのポインタ</param>
	virtual void Update(IBaseTankEnemy* enemy) = 0;
	/// <summary>
	/// 切り替え時の最後の処理
	/// </summary>
	/// <param name="enemy">タンクエネミーのポインタ</param>
	virtual void Exit(IBaseTankEnemy* enemy) = 0;

protected:
	/// ============================== ///
	///		公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// 死亡処理への遷移
	/// </summary>
	/// <param name="enemy">タンクエネミーのポインタ</param>
	void TransitionDeadState(IBaseTankEnemy* enemy);
	/// <summary>
	/// 回転の更新処理
	/// </summary>
	/// <param name="enemy">タンクエネミーのポインタ</param>
	void UpdateRotate(IBaseTankEnemy* enemy);
};

