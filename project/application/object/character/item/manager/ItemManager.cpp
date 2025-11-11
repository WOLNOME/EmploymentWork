#include "ItemManager.h"
#include <random>

void ItemManager::Initialize() {
	//アイテムのパラメーターを読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/item");
}

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
	//確率でアイテムを生成
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0.0f, 1.0f); // 0から1の範囲で乱数を生成
	float dropRate = param_["dropRate"];
	if (dist(mt) >= dropRate) {
		return;
	}

	// アイテムの生成
	std::unique_ptr<Item> item = std::make_unique<Item>();
	item->Initialize();
	item->SetInitPos(_initPos);
	items_.push_back(std::move(item)); // アイテムをコンテナに追加
}
