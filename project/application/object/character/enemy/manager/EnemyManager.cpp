#include "EnemyManager.h"
#include <LevelLoader.h>
#include <SceneManager.h>
#include <TextureManager.h>

//アプリケーション
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

	//↓仮処理

	//ボスの初期化
	boss_ = std::make_unique<Boss>();
	boss_->Initialize();

	//ボスをスポーンさせる
	boss_->Spawn({ 0.0f,0.0f,-500.0f });
	boss_->SetRotate({ 0.0f,3.14f,0.0f });
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
	//ボスの更新
	{
		//アイドル状態なら次へ
		if (boss_->GetState() == BaseCharacter::State::kIdle) {
			return;
		}
		boss_->Update();
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
	boss_->DebugWithImGui();
#endif // _DEBUG
}

void EnemyManager::SetLevelLoader(LevelLoader* _levelLoader) {
	//レベルローダーからキャノ太のスポーンデータを取得
	const auto& canotaSpawnData = _levelLoader->GetEnemySpawnData();
	for (const auto& data : canotaSpawnData) {
		if (data.fileName != "canota") {
			continue; // ファイル名が"canota"でない場合はスキップ
		}
		//コンテナを走査
		for (int i = 0; i < param_["maxCanotaNum"]; i++) {
			//アイドル状態の要素を見つけたら
			if (canotas_[i]->GetState() == BaseCharacter::State::kIdle) {
				//スポーンさせる
				canotas_[i]->Spawn(data.translation, data.rotation);

				break;
			}
		}
	}
	//レベルローダーからキーキャノ太のスポーンデータを取得
	const auto& keyCanotaSpawnData = _levelLoader->GetEnemySpawnData();
	for (const auto& data : keyCanotaSpawnData) {
		if (data.fileName != "keyCanota") {
			continue; // ファイル名が"canota"でない場合はスキップ
		}
		//コンテナを走査
		for (int i = 0; i < param_["maxKeyCanotaNum"]; i++) {
			//アイドル状態の要素を見つけたら
			if (keyCanotas_[i]->GetState() == BaseCharacter::State::kIdle) {
				//スポーンさせる
				keyCanotas_[i]->Spawn(data.translation, data.rotation);

				break;
			}
		}
	}
	//レベルローダーからジェットのスポーンデータを取得

	//コンテナを走査
	for (int i = 0; i < param_["maxJetNum"]; i++) {
		//アイドル状態の要素を見つけたら
		if (jets_[i]->GetState() == BaseCharacter::State::kIdle) {
			//スポーンさせる
			jets_[i]->Spawn({ 0.0f,40.0f,400.0f }, { 0.0f,0.0f,0.0f });

			break;
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
	boss_->SetPlayer(_player);
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
	boss_->SetEnemyWeaponManager(_enemyWeaponManager);
}

void EnemyManager::SetMessageUI(MessageUI* _messageUI) {
	for (const auto& jet : jets_) {
		jet->SetMessageUI(_messageUI);
	}
}
