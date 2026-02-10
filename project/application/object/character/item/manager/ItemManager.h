#pragma once
#include <vector>
#include <string>
#include <Vector3.h>
#include <memory>
#include <JsonUtil.h>

//アプリケーション
#include <application/object/character/item/heal/ItemHeal.h>
#include <application/object/character/item/key/ItemKey.h>

/// <summary>
/// シーン上すべてのアイテム管理を行うクラス
/// </summary>
class ItemManager {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ItemManager() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ItemManager() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui();

	/// <summary>
	/// 回復アイテムのスポーン
	/// </summary>
	/// <param name="_initPos">初期化用座標</param>
	void SpawnHealItem(const Norm::Vector3& _initPos);
	/// <summary>
	/// キーアイテムのスポーン
	/// </summary>
	/// <param name="_initPos">初期化用座標</param>
	void SpawnKeyItem(const Norm::Vector3& _initPos);

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// 回復アイテムのコンテナを取得
	/// </summary>
	/// <returns>回復アイテムのコンテナ</returns>
	const std::vector<std::unique_ptr<ItemHeal>>& GetHealItems() const { return healItems_; }
	/// <summary>
	/// キーアイテムのコンテナを取得
	/// </summary>
	/// <returns>キーアイテムのコンテナ</returns>
	const std::vector<std::unique_ptr<ItemKey>>& GetKeyItems() const { return keyItems_; }

private:
	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//アイテムのパラメーター
	json param_;

	//回復アイテムのコンテナ
	std::vector<std::unique_ptr<ItemHeal>> healItems_;
	//キーアイテムのコンテナ
	std::vector<std::unique_ptr<ItemKey>> keyItems_;

};

