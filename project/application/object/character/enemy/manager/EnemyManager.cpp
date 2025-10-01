#include "EnemyManager.h"
#include <LevelLoader.h>
#include <SceneManager.h>

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
	//全ボスの死亡時処理
	for (auto it = bosses_.begin(); it != bosses_.end();) {
		if ((*it)->GetIsDead()) {
			it = bosses_.erase(it);
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
	//全ボスの更新
	for (const auto& boss : bosses_) {
		boss->Update();
	}
	//全ジェットの更新
	for (const auto& jet : jets_) {
		jet->Update();
	}

	//もし全てのボスが死亡していたら
	if (bosses_.empty()) {
		//クリアシーンの移行
		SceneManager::GetInstance()->SetNextScene("GameClear");
	}
}

void EnemyManager::DebugWithImGui() {
#ifdef _DEBUG
	//全キャノ太のデバッグ処理
	for (const auto& canota : canotas_) {
		canota->DebugWithImGui();
	}
	//全ボスのデバッグ処理
	for (const auto& boss : bosses_) {
		boss->DebugWithImGui();
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
	//レベルローダーからボスのスポーンデータを取得
	const auto& bossSpawnData = _levelLoader->GetEnemySpawnData();
	for (const auto& data : bossSpawnData) {
		if (data.fileName != "boss") {
			continue; // ファイル名が"canota"でない場合はスキップ
		}
		std::unique_ptr<Boss> boss = nullptr;
		boss = std::make_unique<Boss>(true);
		boss->Initialize();
		boss->SetTranslate(data.translation);
		boss->SetRotate(data.rotation);
		bosses_.push_back(std::move(boss));
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
	player_ = _player;
	for (const auto& canota : canotas_) {
		canota->SetPlayer(player_);
	}
	for (const auto& boss : bosses_) {
		boss->SetPlayer(player_);
	}
	for (const auto& jet : jets_) {
		jet->SetPlayer(player_);
	}
}

void EnemyManager::SetItemManager(ItemManager* _itemManager) {
	itemManager_ = _itemManager;
	for (const auto& canota : canotas_) {
		canota->SetItemManager(itemManager_);
	}
	for (const auto& boss : bosses_) {
		boss->SetItemManager(itemManager_);
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
