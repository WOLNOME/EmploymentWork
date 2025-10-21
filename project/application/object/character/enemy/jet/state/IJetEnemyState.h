#pragma once

class IBaseJetEnemy;

/// <summary>
/// ジェットエネミーの状態を表すインターフェースクラス
/// 状態の遷移や更新処理を定義する
/// </summary>
class IJetEnemyState {
	public:
	virtual ~IJetEnemyState() = default;
	virtual void Enter(IBaseJetEnemy* enemy) = 0;
	virtual void Update(IBaseJetEnemy* enemy) = 0;
	virtual void Exit(IBaseJetEnemy* enemy) = 0;

protected:
	void TransitionDeadState(IBaseJetEnemy* enemy);
};

