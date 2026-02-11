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
	//必殺弾チャージアイテムコンテナの生成・初期化
	int chargeItemNum = param_["maxChargeItemNum"];
	for (int i = 0; i < chargeItemNum; i++) {
		chargeItems_.push_back(std::make_unique<ItemCharge>());
		chargeItems_[i]->Initialize();
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
	// 必殺弾チャージアイテムの更新
	for (const auto& chargeItem : chargeItems_) {
		//アイドル状態の要素はスキップ
		if (chargeItem->GetState() == BaseCharacter::State::kIdle) {
			continue;
		}
		chargeItem->Update();
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
	//スポーン
	SpawnFromPool(healItems_, [&](ItemHeal* heal) {
		heal->Spawn(_initPos);
		});
}

void ItemManager::SpawnChargeItem(const Norm::Vector3& _initPos) {
	//スポーン
	SpawnFromPool(chargeItems_, [&](ItemCharge* charge) {
		charge->Spawn(_initPos);
		});
}

void ItemManager::SpawnKeyItem(const Norm::Vector3& _initPos) {
	//スポーン
	SpawnFromPool(keyItems_, [&](ItemKey* key) {
		key->Spawn(_initPos);
		});
}
