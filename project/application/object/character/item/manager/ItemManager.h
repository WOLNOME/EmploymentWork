#pragma once
#include <list>
#include <string>
#include <Vector3.h>
#include <memory>

//アプリケーション
#include <application/object/character/item/Item.h>


class ItemManager {
public:
	// コンストラクタ
	ItemManager() = default;
	// デストラクタ
	~ItemManager() = default;

	//更新
	void Update();

	// デバッグ用パラメーター調整
	void DebugWithImGui();

	// アイテムの追加
	void AddItem(const Vector3& _initPos);

private:
	//アイテムのコンテナ
	std::list<std::unique_ptr<Item>> items_;
};

