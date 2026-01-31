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
class MessageUI;

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
	virtual void Spawn(const Vector3& _initPos, const Vector3& _initRotate) = 0;

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
	/// メッセージUIを設定する
	/// </summary>
	/// <param name="messageUI">設定するメッセージUI</param>
	void SetMessageUI(MessageUI* messageUI);

protected:
	/// ============================== ///
	///		非公開メンバ関数
	///	============================== ///

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="attribute">相手の属性</param>
	/// <param name="subjectPos">相手の座標</param>
	void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) override;


	/// ============================== ///
	///		インスタンス
	///	============================== ///

	Player* player_ = nullptr;
	ItemManager* itemManager_ = nullptr;

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