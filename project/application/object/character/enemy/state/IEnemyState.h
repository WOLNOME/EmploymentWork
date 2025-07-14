#pragma once

class IBaseEnemy;
class IEnemyState {
public:
	virtual ~IEnemyState() = default;

	virtual void Enter(IBaseEnemy* enemy) = 0;
	virtual void Update(IBaseEnemy* enemy) = 0;
	virtual void Exit(IBaseEnemy* enemy) = 0;

protected:
	void TransitionDeadState(IBaseEnemy* enemy);
	void UpdateRotate(IBaseEnemy* enemy);
};

