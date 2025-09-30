#pragma once

class IBaseTankEnemy;
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

