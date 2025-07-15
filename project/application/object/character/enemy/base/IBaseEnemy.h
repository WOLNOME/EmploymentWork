#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "Particle.h"
#include "JsonUtil.h"
#include <Vector3.h>
#include <string>
#include <memory>

//アプリケーション
#include "../state/IEnemyState.h"
#include "../state/EnemyPatrolState.h"
#include "../state/EnemyApproachState.h"
#include "../state/EnemyAttackState.h"
#include "../state/EnemyDeadState.h"

class Player;
class IBaseEnemy : public BaseCharacter {
public:
	//コンストラクタ
	IBaseEnemy(bool _isUseCannon);
	// デストラクタ
	~IBaseEnemy() override = default;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui() override;

public://getter
	Player* GetPlayer() { return player_; }
	const json& GetParam() { return param_; }
	int GetMaxHP() const { return maxHP_; }
	int GetHP() const { return hp_; }

	EnemyAttackState* GetAttackState() const { return attackState_.get(); }

public://setter
	void SetPlayer(Player* _player) { player_ = _player; }

protected:
	//当たり判定処理
	void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) override;

public://状態管理用関数
	void ChangeState(const std::string& stateName);

protected://借用インスタンス
	Player* player_ = nullptr;

protected://メンバ変数
	//パラメーター
	json param_;

	//HP
	int maxHP_;	//最大HP
	int hp_;	//現在のHP

private:
	//状態管理用変数
	IEnemyState* currentState_ = nullptr;

	std::unique_ptr<EnemyPatrolState> patrolState_;
	std::unique_ptr<EnemyApproachState> approachState_;
	std::unique_ptr<EnemyAttackState> attackState_;
	std::unique_ptr<EnemyDeadState> deadState_;

};

