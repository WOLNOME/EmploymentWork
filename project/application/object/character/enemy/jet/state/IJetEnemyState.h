#pragma once

class IBaseJetEnemy;
class IJetEnemyState {
	public:
	virtual ~IJetEnemyState() = default;
	virtual void Enter(IBaseJetEnemy* enemy) = 0;
	virtual void Update(IBaseJetEnemy* enemy) = 0;
	virtual void Exit(IBaseJetEnemy* enemy) = 0;

protected:
	void TransitionDeadState(IBaseJetEnemy* enemy);
};

