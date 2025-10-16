#pragma once

class IBaseTankEnemy;

/// <summary>
/// 戦車型敵キャラクターの状態を表すインターフェースクラス
/// 状態の遷移や更新処理を定義する
/// </summary>
class ITankEnemyState {
public:
	virtual ~ITankEnemyState() = default;

	virtual void Enter(IBaseTankEnemy* enemy) = 0;
	virtual void Update(IBaseTankEnemy* enemy) = 0;
	virtual void Exit(IBaseTankEnemy* enemy) = 0;

protected:
	void TransitionDeadState(IBaseTankEnemy* enemy);
	void UpdateRotate(IBaseTankEnemy* enemy);
};

