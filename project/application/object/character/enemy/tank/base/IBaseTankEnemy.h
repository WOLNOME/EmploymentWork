#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "JsonUtil.h"
#include "CombinedParticle.h"
#include <Vector3.h>
#include <string>
#include <memory>

//アプリケーション
#include "../state/ITankEnemyState.h"
#include "../state/TankEnemyPatrolState.h"
#include "../state/TankEnemyApproachState.h"
#include "../state/TankEnemyAttackState.h"
#include "../state/TankEnemyDeadState.h"

//前方宣言
class Player;
class ItemManager;

/// <summary>
/// 戦車型エネミー専用の基底クラス
/// </summary>
class IBaseTankEnemy : public BaseCharacter {
public:
	/// ============================== ///
	///		列挙体
	/// ============================== ///
	enum class StateName {
		kPatrol,		//巡回
		kApproach,		//接近
		kAttack,		//攻撃
		kDead,			//死亡
	};

	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_isUseCannon">砲弾の使用フラグ</param>
	IBaseTankEnemy(bool _isUseCannon);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~IBaseTankEnemy() override = default;
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
	/// ============================== ///

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
	/// 現在の状態名を取得する
	/// </summary>
	/// <returns>現在の状態名</returns>
	StateName GetCurrentStateName() const { return currentStateName_; }
	/// <summary>
	/// 攻撃ステートを取得する
	/// </summary>
	/// <returns>攻撃ステート</returns>
	TankEnemyAttackState* GetAttackState() const { return attackState_.get(); }

	/// ============================== ///
	///		setter
	/// ============================== ///

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

protected:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="attribute">相手の属性</param>
	/// <param name="subjectPos">相手の座標</param>
	void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) override;

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	Player* player_ = nullptr;
	ItemManager* itemManager_ = nullptr;
	//移動パーティクル
	std::unique_ptr<CombinedParticle> moveParticle_ = nullptr;

	/// ============================== ///
	///		メンバ変数(protected)
	/// ============================== ///

	//パラメーター
	json param_;

	//HP
	int maxHP_;	//最大HP
	int hp_;	//現在のHP

private:
	/// ============================== ///
	///		メンバ変数(private)
	/// ============================== ///

	//状態管理用変数
	StateName currentStateName_;	//現在の状態名
	ITankEnemyState* currentState_ = nullptr;

	std::unique_ptr<TankEnemyPatrolState> patrolState_;
	std::unique_ptr<TankEnemyApproachState> approachState_;
	std::unique_ptr<TankEnemyAttackState> attackState_;
	std::unique_ptr<TankEnemyDeadState> deadState_;

};

