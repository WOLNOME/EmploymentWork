#pragma once
#include <vector>
#include <string>
#include <Vector3.h>
#include <memory>
#include <JsonUtil.h>

//アプリケーション
#include <application/object/character/item/Item.h>

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
	/// アイテムの追加
	/// </summary>
	/// <param name="_initPos">初期化用座標</param>
	void AddItem(const Vector3& _initPos);

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// アイテムのコンテナを取得
	/// </summary>
	/// <returns>アイテムのコンテナ</returns>
	const std::vector<std::unique_ptr<Item>>& GetItems() const { return items_; }

private:
	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//アイテムのパラメーター
	json param_;

	//アイテムのコンテナ
	std::vector<std::unique_ptr<Item>> items_;
};

