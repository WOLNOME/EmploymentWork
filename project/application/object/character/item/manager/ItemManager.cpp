#include "ItemManager.h"
#include <random>

void ItemManager::Update() {
	//死亡したアイテムを削除
	for (auto it = items_.begin(); it != items_.end();) {
		if ((*it)->GetIsDead()) {
			it = items_.erase(it); // アイテムを削除
		}
		else {
			++it; // 次のアイテムへ
		}
	}

	// アイテムの更新
	for (const auto& item : items_) {
		item->Update();
	}
}

void ItemManager::DebugWithImGui() {
#ifdef _DEBUG
	// 各アイテムのデバッグ情報を表示
	for (const auto& item : items_) {
		item->DebugWithImGui();
	}
#endif // _DEBUG
}

void ItemManager::AddItem(const Vector3& _initPos) {
	//80%の確率でアイテムを生成
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, 9);
	if (dist(mt) >= 8) {
		return; // 20%の確率でアイテムを生成しない
	}

	// アイテムの生成
	std::unique_ptr<Item> item = std::make_unique<Item>();
	item->Initialize(_initPos);
	items_.push_back(std::move(item)); // アイテムをコンテナに追加
}
