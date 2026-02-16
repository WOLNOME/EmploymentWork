#include "EnemyManager.h"
#include <SceneManager.h>
#include <TextureManager.h>

//アプリケーション
#include "application/object/level/loader/LevelLoader.h"
#include "application/object/character/player/Player.h"
#include "application/object/character/item/manager/ItemManager.h"

using namespace Norm;

void EnemyManager::Initialize() {
	//パラメーターの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/enemyManager");

	//キャノ太の生成と初期化
	int canotaNum = param_["maxCanotaNum"];
	for (int i = 0; i < canotaNum; i++) {
		canotas_.push_back(std::make_unique<Canota>());
		canotas_[i]->Initialize();
	}
	//キーキャノ太の生成と初期化
	int keyCanotaNum = param_["maxKeyCanotaNum"];
	for (int i = 0; i < keyCanotaNum; i++) {
		keyCanotas_.push_back(std::make_unique<KeyCanota>());
		keyCanotas_[i]->Initialize();
	}
	//ジェットの生成と初期化
	int jetNum = param_["maxJetNum"];
	for (int i = 0; i < jetNum; i++) {
		jets_.push_back(std::make_unique<Jet>());
		jets_[i]->Initialize();
	}
	//ボスの生成と初期化
	int bossNum = param_["maxBossNum"];
	for (int i = 0; i < jetNum; i++) {
		bosses_.push_back(std::make_unique<Boss>());
		bosses_[i]->Initialize();
		bosses_[i]->SetEnemyManager(this);
	}

	//↓仮処理

	//ボスをスポーンさせる
	BossSpawn({ 0.0f,0.0f,-500.0f }, { 0.0f,3.14f,0.0f });
}

void EnemyManager::Update() {
	//全キャノ太の更新
	for (const auto& canota : canotas_) {
		//アイドル状態なら次へ
		if (canota->GetState() == BaseCharacter::State::kIdle) {
			continue;
		}

		canota->Update();
	}
	//全キーキャノ太の更新
	for (const auto& keyCanota : keyCanotas_) {
		//アイドル状態なら次へ
		if (keyCanota->GetState() == BaseCharacter::State::kIdle) {
			continue;
		}

		keyCanota->Update();
	}
	//全ジェットの更新
	for (const auto& jet : jets_) {
		//アイドル状態なら次へ
		if (jet->GetState() == BaseCharacter::State::kIdle) {
			continue;
		}

		jet->Update();
	}
	//全ボスの更新
	for (const auto& boss : bosses_) {
		//アイドル状態なら次へ
		if (boss->GetState() == BaseCharacter::State::kIdle) {
			continue;
		}

		boss->Update();
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
	//ボスのデバッグ処理
	for (const auto& boss : bosses_) {
		boss->DebugWithImGui();
	}
#endif // _DEBUG
}

void EnemyManager::CanotaSpawn(const Norm::Vector3& _initPos, const Norm::Vector3& _initRotate) {
	//スポーン
	SpawnFromPool(canotas_, [&](Canota* canota) {
		canota->Spawn(_initPos, _initRotate);
		});
}

void EnemyManager::KeyCanotaSpawn(const Norm::Vector3& _initPos, const Norm::Vector3& _initRotate) {
	//スポーン
	SpawnFromPool(keyCanotas_, [&](KeyCanota* keyCanota) {
		keyCanota->Spawn(_initPos, _initRotate);
		});
}

void EnemyManager::JetSpawn(const Norm::Vector3& _initPos, const Norm::Vector3& _initRotate) {
	//スポーン
	SpawnFromPool(jets_, [&](Jet* jet) {
		jet->Spawn(_initPos, _initRotate);
		});
}

void EnemyManager::BossSpawn(const Norm::Vector3& _initPos, const Norm::Vector3& _initRotate) {
	//スポーン
	SpawnFromPool(bosses_, [&](Boss* boss) {
		boss->Spawn(_initPos, _initRotate);
		});
}

void EnemyManager::SetLevelLoader(LevelLoader* _levelLoader) {
	const auto& enemySpawnData = _levelLoader->GetEnemySpawnData();
	//レベルローダーからスポーンデータを取得
	for (const auto& data : enemySpawnData) {
		if (data.type == "Canota") {
			//スポーン
			CanotaSpawn(data.translation, data.rotation);
			continue;
		}
		else if (data.type == "KeyCanota") {
			//スポーン
			KeyCanotaSpawn(data.translation, data.rotation);
			continue;
		}
		else if (data.type == "Jet") {
			//スポーン
			JetSpawn(data.translation, data.rotation);
			continue;
		}
	}
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
	for (const auto& boss : bosses_) {
		boss->SetPlayer(player_);
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

void EnemyManager::SetEnemyWeaponManager(EnemyWeaponManager* _enemyWeaponManager) {
	//全敵に敵武器マネージャーをセットする
	enemyWeaponManager_ = _enemyWeaponManager;
	for (const auto& canota : canotas_) {
		canota->SetEnemyWeaponManager(_enemyWeaponManager);
	}
	for (const auto& keyCanota : keyCanotas_) {
		keyCanota->SetEnemyWeaponManager(_enemyWeaponManager);
	}
	for (const auto& jet : jets_) {
		jet->SetEnemyWeaponManager(_enemyWeaponManager);
	}
	for (const auto& boss : bosses_) {
		boss->SetEnemyWeaponManager(_enemyWeaponManager);
	}
}

void EnemyManager::SetMessageUI(MessageUI* _messageUI) {
	for (const auto& jet : jets_) {
		jet->SetMessageUI(_messageUI);
	}
}

void EnemyManager::SetEnemyUI(EnemyUI* _enemyUI) {
	//全敵に敵UIをセットする
	enemyUI_ = _enemyUI;
	for (const auto& canota : canotas_) {
		canota->SetEnemyUI(enemyUI_);
	}
	for (const auto& keyCanota : keyCanotas_) {
		keyCanota->SetEnemyUI(enemyUI_);
	}
	for (const auto& jet : jets_) {
		jet->SetEnemyUI(enemyUI_);
	}
	for (const auto& boss : bosses_) {
		boss->SetEnemyUI(enemyUI_);
	}
}
