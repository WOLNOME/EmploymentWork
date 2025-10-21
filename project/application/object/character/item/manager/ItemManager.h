#pragma once
#include <list>
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
	// コンストラクタ
	ItemManager() = default;
	// デストラクタ
	~ItemManager() = default;

	//初期化
	void Initialize();
	//更新
	void Update();

	// デバッグ用パラメーター調整
	void DebugWithImGui();

	// アイテムの追加
	void AddItem(const Vector3& _initPos);

public://getter
	//アイテムのコンテナを取得
	const std::list<std::unique_ptr<Item>>& GetItems() const { return items_; }

private:
	//アイテムのパラメーター
	json param_;

	//アイテムのコンテナ
	std::list<std::unique_ptr<Item>> items_;
};

