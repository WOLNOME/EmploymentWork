#include "ItemManager.h"
#include <random>

using namespace Norm;

void ItemManager::Initialize() {
	//アイテムのパラメーターを読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/item");

	//アイテムコンテナの生成・初期化
	int itemNum = param_["maxItemNum"];
	for (int i = 0; i < itemNum; i++) {
		items_.push_back(std::make_unique<Item>());
		items_[i]->Initialize();
	}

}

void ItemManager::Update() {
	// アイテムの更新
	for (const auto& item : items_) {
		//アイドル状態の要素はスキップ
		if (item->GetState() == BaseCharacter::State::kIdle) {
			continue;
		}

		item->Update();
	}
}

void ItemManager::DebugWithImGui() {
#ifdef _DEBUG
	// 各アイテムのデバッグ情報を表示
	for (const auto& item : items_) {
		//アイドル状態の要素はスキップ
		if (item->GetState() == BaseCharacter::State::kIdle) {
			continue;
		}
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

	//アイテムコンテナを走査
	for (auto& item : items_) {
		//アイテムがアイドル状態でなければ次へ
		if (item->GetState() != BaseCharacter::State::kIdle) {
			continue;
		}
		//アイテムをスポーン
		item->Spawn(_initPos);

		break;
	}
}
