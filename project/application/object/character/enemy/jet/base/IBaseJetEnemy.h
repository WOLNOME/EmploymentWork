#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "Particle.h"
#include "JsonUtil.h"
#include <Vector3.h>
#include <string>
#include <memory>

//アプリケーション
#include "../state/IJetEnemyState.h"
#include "../state/JetEnemyPatrolState.h"
#include "../state/JetEnemyApproachState.h"
#include "../state/JetEnemyAttackState.h"
#include "../state/JetEnemyDeadState.h"

class Player;
class ItemManager;
class MessageUI;
class IBaseJetEnemy : public BaseCharacter {
public:
	//コンストラクタ
	IBaseJetEnemy();
	// デストラクタ
	~IBaseJetEnemy() override = default;
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

	JetEnemyAttackState* GetAttackState() const { return attackState_.get(); }

public://setter
	void SetPlayer(Player* _player) { player_ = _player; }
	void SetItemManager(ItemManager* _itemManager) { itemManager_ = _itemManager; }
	void SetMessageUI(MessageUI* messageUI);

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
	IJetEnemyState* currentState_ = nullptr;

	std::unique_ptr<JetEnemyPatrolState> patrolState_;
	std::unique_ptr<JetEnemyApproachState> approachState_;
	std::unique_ptr<JetEnemyAttackState> attackState_;
	std::unique_ptr<JetEnemyDeadState> deadState_;

};