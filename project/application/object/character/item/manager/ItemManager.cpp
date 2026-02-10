#include "ItemManager.h"
#include <random>

using namespace Norm;

void ItemManager::Initialize() {
	//アイテムのパラメーターを読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/item");

	//回復アイテムコンテナの生成・初期化
	int healItemNum = param_["maxHealItemNum"];
	for (int i = 0; i < healItemNum; i++) {
		healItems_.push_back(std::make_unique<ItemHeal>());
		healItems_[i]->Initialize();
	}
	//キーアイテムコンテナの生成・初期化
	int keyItemNum = param_["maxKeyItemNum"];
	for (int i = 0; i < healItemNum; i++) {
		keyItems_.push_back(std::make_unique<ItemKey>());
		keyItems_[i]->Initialize();
	}
}

void ItemManager::Update() {
	// 回復アイテムの更新
	for (const auto& healItem : healItems_) {
		//アイドル状態の要素はスキップ
		if (healItem->GetState() == BaseCharacter::State::kIdle) {
			continue;
		}
		healItem->Update();
	}
	// キーアイテムの更新
	for (const auto& keyItem : keyItems_) {
		//アイドル状態の要素はスキップ
		if (keyItem->GetState() == BaseCharacter::State::kIdle) {
			continue;
		}

		keyItem->Update();
	}
}

void ItemManager::DebugWithImGui() {
#ifdef _DEBUG
	
#endif // _DEBUG
}

void ItemManager::SpawnHealItem(const Vector3& _initPos) {
	//確率でアイテムを生成
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(0.0f, 1.0f); // 0から1の範囲で乱数を生成
	float dropRate = param_["dropRate"];
	if (dist(mt) >= dropRate) {
		return;
	}

	//回復アイテムコンテナを走査
	for (auto& healItem : healItems_) {
		//回復アイテムがアイドル状態でなければ次へ
		if (healItem->GetState() != BaseCharacter::State::kIdle) {
			continue;
		}
		//回復アイテムをスポーン
		healItem->Spawn(_initPos);

		break;
	}
}

void ItemManager::SpawnKeyItem(const Norm::Vector3& _initPos) {
	//キーアイテムコンテナを走査
	for (auto& keyItem : keyItems_) {
		//回復アイテムがアイドル状態でなければ次へ
		if (keyItem->GetState() != BaseCharacter::State::kIdle) {
			continue;
		}
		//回復アイテムをスポーン
		keyItem->Spawn(_initPos);

		break;
	}
}
