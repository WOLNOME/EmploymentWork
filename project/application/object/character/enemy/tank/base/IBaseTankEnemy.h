#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "JsonUtil.h"
#include <Vector3.h>
#include <string>
#include <memory>

//アプリケーション
#include "../state/ITankEnemyState.h"
#include "../state/TankEnemyPatrolState.h"
#include "../state/TankEnemyApproachState.h"
#include "../state/TankEnemyAttackState.h"
#include "../state/TankEnemyDeadState.h"

class Player;
class ItemManager;

/// <summary>
/// 戦車型エネミー専用の基底クラス
/// </summary>
class IBaseTankEnemy : public BaseCharacter {
public:
	//コンストラクタ
	IBaseTankEnemy(bool _isUseCannon);
	// デストラクタ
	~IBaseTankEnemy() override = default;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
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

	TankEnemyAttackState* GetAttackState() const { return attackState_.get(); }

public://setter
	void SetPlayer(Player* _player) { player_ = _player; }
	void SetItemManager(ItemManager* _itemManager) { itemManager_ = _itemManager; }

protected:
	//当たり判定処理
	void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) override;

public://状態管理用関数
	void ChangeState(const std::string& stateName);

protected://借用インスタンス
	Player* player_ = nullptr;
	ItemManager* itemManager_ = nullptr;

protected://メンバ変数
	//パラメーター
	json param_;

	//HP
	int maxHP_;	//最大HP
	int hp_;	//現在のHP

private:
	//状態管理用変数
	ITankEnemyState* currentState_ = nullptr;

	std::unique_ptr<TankEnemyPatrolState> patrolState_;
	std::unique_ptr<TankEnemyApproachState> approachState_;
	std::unique_ptr<TankEnemyAttackState> attackState_;
	std::unique_ptr<TankEnemyDeadState> deadState_;

};

