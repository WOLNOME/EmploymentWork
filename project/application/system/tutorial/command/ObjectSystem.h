#pragma once
#include <Vector3.h>

// 前方宣言
class Player;
class ItemManager;

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
	void SetPlayer(Player* player) { player = player; }
	/// <summary>
	/// アイテムマネージャーインスタンスを設定する
	/// </summary>
	/// <param name="itemManager">アイテムマネージャーへのポインタ</param>
	void SetItemManager(ItemManager* itemManager) { itemManager_ = itemManager; }

private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	Player* player_ = nullptr;          // プレイヤー参照
	ItemManager* itemManager_ = nullptr; // アイテム管理クラス参照
};
