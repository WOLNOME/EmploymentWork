#include "EnemyManager.h"
#include <LevelLoader.h>
#include <SceneManager.h>
#include <TextureManager.h>

//アプリケーション
#include "application/object/character/player/Player.h"
#include "application/object/character/item/manager/ItemManager.h"

void EnemyManager::Initialize() {
}

void EnemyManager::Update() {
	//全キャノ太の死亡時処理
	for (auto it = canotas_.begin(); it != canotas_.end();) {
		if ((*it)->GetIsDead()) {
			it = canotas_.erase(it);
		}
		else {
			++it;
		}
	}
	//全キーキャノ太の死亡時処理
	for (auto it = keyCanotas_.begin(); it != keyCanotas_.end();) {
		if ((*it)->GetIsDead()) {
			it = keyCanotas_.erase(it);
		}
		else {
			++it;
		}
	}
	//全ジェットの死亡時処理
	for (auto it = jets_.begin(); it != jets_.end();) {
		if ((*it)->GetIsDead()) {
			it = jets_.erase(it);
		}
		else {
			++it;
		}
	}

	//全キャノ太の更新
	for (const auto& canota : canotas_) {
		canota->Update();
	}
	//全キーキャノ太の更新
	for (const auto& keyCanota : keyCanotas_) {
		keyCanota->Update();
	}
	//全ジェットの更新
	for (const auto& jet : jets_) {
		jet->Update();
	}
}

void EnemyManager::DebugWithImGui() {
#ifdef _DEBUG
	//全キャノ太のデバッグ処理
	for (const auto& canota : canotas_) {
		canota->DebugWithImGui();
	}
	//全キーキャノ太のデバッグ処理
	for (const auto& keyCanota : keyCanotas_) {
		keyCanota->DebugWithImGui();
	}
	//全ジェットのデバッグ処理
	for (const auto& jet : jets_) {
		jet->DebugWithImGui();
	}
#endif // _DEBUG
}

void EnemyManager::SetLevelLoader(LevelLoader* _levelLoader) {
	//レベルローダーからキャノ太のスポーンデータを取得
	const auto& canotaSpawnData = _levelLoader->GetEnemySpawnData();
	for (const auto& data : canotaSpawnData) {
		if (data.fileName != "canota") {
			continue; // ファイル名が"canota"でない場合はスキップ
		}
		std::unique_ptr<Canota> canota = nullptr;
		canota = std::make_unique<Canota>(true);
		canota->Initialize();
		canota->SetTranslate(data.translation);
		canota->SetRotate(data.rotation);
		canotas_.push_back(std::move(canota));
	}
	//レベルローダーからキーキャノ太のスポーンデータを取得
	const auto& keyCanotaSpawnData = _levelLoader->GetEnemySpawnData();
	for (const auto& data : keyCanotaSpawnData) {
		if (data.fileName != "keyCanota") {
			continue; // ファイル名が"canota"でない場合はスキップ
		}
		std::unique_ptr<KeyCanota> keyCanota = nullptr;
		keyCanota = std::make_unique<KeyCanota>(true);
		keyCanota->Initialize();
		keyCanota->SetTranslate(data.translation);
		keyCanota->SetRotate(data.rotation);
		keyCanotas_.push_back(std::move(keyCanota));
	}
	//レベルローダーからジェットのスポーンデータを取得
	std::unique_ptr<Jet> jet = nullptr;
	jet = std::make_unique<Jet>();
	jet->Initialize();
	jet->SetTranslate({ 0.0f,40.0f,400.0f });
	jet->SetRotate({ 0.0f,0.0f,0.0f });
	jets_.push_back(std::move(jet));


}

void EnemyManager::SetPlayer(Player* _player) {
	//全敵にプレイヤーをセットする
	player_ = _player;
	for (const auto& canota : canotas_) {
		canota->SetPlayer(player_);
	}
	for (const auto& keyCanota : keyCanotas_) {
		keyCanota->SetPlayer(player_);
	}
	for (const auto& jet : jets_) {
		jet->SetPlayer(player_);
	}
}

void EnemyManager::SetItemManager(ItemManager* _itemManager) {
	//全敵にアイテムマネージャーをセットする
	itemManager_ = _itemManager;
	for (const auto& canota : canotas_) {
		canota->SetItemManager(itemManager_);
	}
	for (const auto& keyCanota : keyCanotas_) {
		keyCanota->SetItemManager(itemManager_);
	}
	for (const auto& jet : jets_) {
		jet->SetItemManager(itemManager_);
	}
}

void EnemyManager::SetMessageUI(MessageUI* _messageUI) {
	for (const auto& jet : jets_) {
		jet->SetMessageUI(_messageUI);
	}
}
