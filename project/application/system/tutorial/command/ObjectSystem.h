#pragma once
#include <Vector3.h>

// 前方宣言
class Player;
class ItemManager;
class EnemyManager;

/// <summary>
/// オブジェクトシステム
/// </summary>
class ObjectSystem {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 指定位置・回転でアイテムを生成する
	/// </summary>
	/// <param name="pos">生成位置</param>
	void SpawnItem(const Norm::Vector3& pos);

	/// <summary>
	/// 敵を生成する
	/// </summary>
	/// <param name="enemyKind">敵の種類</param>
	/// <param name="pos">座標</param>
	/// <param name="rotate">回転</param>
	void SpawnEnemy(const std::string& enemyKind, const Norm::Vector3& pos, const Norm::Vector3& rotate);

	/// <summary>
	/// 全エネミーのクリア
	/// </summary>
	void ClearEnemy();

	/// <summary>
	/// 敵の稼働・停止を切り替える
	/// </summary>
	/// <param name="isActive">アクティブかどうか</param>
	void SetIsActiveEnemy(bool isActive);

	/// <summary>
	/// 敵の無敵モードを切り替える
	/// </summary>
	/// <param name="isInvicible">無敵モードかどうか</param>
	void SetIsInvicibleEnemy(bool isInvicible);

	/// <summary>
	/// プレイヤーを指定位置・回転で固定する
	/// </summary>
	/// <param name="pos">固定する位置</param>
	/// <param name="rot">固定する回転</param>
	void FixPlayer(const Norm::Vector3& pos, const Norm::Vector3& rot);

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// プレイヤーインスタンスを設定する
	/// </summary>
	/// <param name="player">プレイヤーへのポインタ</param>
	void SetPlayer(Player* player) { player_ = player; }
	/// <summary>
	/// アイテムマネージャーインスタンスを設定する
	/// </summary>
	/// <param name="itemManager">アイテムマネージャーへのポインタ</param>
	void SetItemManager(ItemManager* itemManager) { itemManager_ = itemManager; }
	/// <summary>
	/// エネミーマネージャーインスタンスを設定する
	/// </summary>
	/// <param name="enemyManager">エネミーマネージャーへのポインタ</param>
	void SetEnemyManager(EnemyManager* enemyManager) { enemyManager_ = enemyManager; }

private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	Player* player_ = nullptr;          // プレイヤー参照
	ItemManager* itemManager_ = nullptr; // アイテム管理クラス参照
	EnemyManager* enemyManager_ = nullptr;	// エネミー管理クラス参照
};
