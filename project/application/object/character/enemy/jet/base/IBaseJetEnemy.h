#pragma once
#include "application/object/character/base/BaseCharacter.h"
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
class EnemyWeaponManager;
class MessageUI;
class EnemyUI;

/// <summary>
/// ジェットエネミー専用の基底クラス
/// </summary>
class IBaseJetEnemy : public BaseCharacter {
public:
	/// ============================== ///
	///		メンバ関数
	///	============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IBaseJetEnemy();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~IBaseJetEnemy() override = default;
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

	/// <summary>
	/// スポーン
	/// </summary>
	/// <param name="_initPos">初期位置</param>
	/// <param name="_initRotate">初期回転</param>
	virtual void Spawn(const Norm::Vector3& _initPos, const Norm::Vector3& _initRotate) = 0;

	/// <summary>
	/// 状態変更用関数
	/// </summary>
	/// <param name="stateName">状態名</param>
	void ChangeState(const std::string& stateName);

	/// ============================== ///
	///		getter
	///	============================== ///

	/// <summary>
	/// プレイヤーを取得する
	/// </summary>
	/// <returns>プレイヤーオブジェクト</returns>
	Player* GetPlayer() { return player_; }
	/// <summary>
	/// 敵武器マネージャーを取得する
	/// </summary>
	/// <returns>敵武器マネージャー</returns>
	EnemyWeaponManager* GetEnemyWeaponManager() { return enemyWeaponManager_; }
	/// <summary>
	/// 敵パラメータ(json)を取得する
	/// </summary>
	/// <returns>敵パラメータ(json)</returns>
	const json& GetParam() { return param_; }
	/// <summary>
	/// 最大HPを取得する
	/// </summary>
	/// <returns>最大HP</returns>
	int GetMaxHP() const { return maxHP_; }
	/// <summary>
	/// 現在のHPを取得する
	/// </summary>
	/// <returns>現在のHP</returns>
	int GetHP() const { return hp_; }
	/// <summary>
	/// 攻撃ステートを取得する
	/// </summary>
	/// <returns>攻撃ステート</returns>
	JetEnemyAttackState* GetAttackState() const { return attackState_.get(); }

	/// ============================== ///
	///		setter
	///	============================== ///

	/// <summary>
	/// プレイヤーを設定する
	/// </summary>
	/// <param name="_player">設定するプレイヤー</param>
	void SetPlayer(Player* _player) { player_ = _player; }
	/// <summary>
	/// アイテムマネージャーを設定する
	/// </summary>
	/// <param name="_itemManager">設定するアイテムマネージャー</param>
	void SetItemManager(ItemManager* _itemManager) { itemManager_ = _itemManager; }
	/// <summary>
	/// 敵武器マネージャーを設定する
	/// </summary>
	/// <param name="_enemyWeaponManager">設定する敵武器マネージャー</param>
	void SetEnemyWeaponManager(EnemyWeaponManager* _enemyWeaponManager) { enemyWeaponManager_ = _enemyWeaponManager; }
	/// <summary>
	/// メッセージUIを設定する
	/// </summary>
	/// <param name="messageUI">設定するメッセージUI</param>
	void SetMessageUI(MessageUI* messageUI);
	/// <summary>
	/// 敵UIを設定する
	/// </summary>
	/// <param name="_enemyUI">設定する敵UI</param>
	void SetEnemyUI(EnemyUI* _enemyUI);

	/// <summary>
	/// HPをセット
	/// </summary>
	/// <param name="_hp">HP</param>
	void SetHP(int _hp) { hp_ = _hp; }

protected:
	/// ============================== ///
	///		インスタンス
	///	============================== ///

	Player* player_ = nullptr;
	ItemManager* itemManager_ = nullptr;
	EnemyWeaponManager* enemyWeaponManager_ = nullptr;
	EnemyUI* enemyUI_ = nullptr;

	/// ============================== ///
	///		メンバ変数(protected)
	///	============================== ///

	//パラメーター
	json param_;

	//HP
	int maxHP_;	//最大HP
	int hp_;	//現在のHP

private:
	/// ============================== ///
	///		メンバ変数(private)
	///	============================== ///

	//状態管理用変数
	IJetEnemyState* currentState_ = nullptr;

	std::unique_ptr<JetEnemyPatrolState> patrolState_;
	std::unique_ptr<JetEnemyApproachState> approachState_;
	std::unique_ptr<JetEnemyAttackState> attackState_;
	std::unique_ptr<JetEnemyDeadState> deadState_;

};