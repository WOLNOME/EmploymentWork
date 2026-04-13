#include "ObjectSystem.h"
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>
#include <application/object/character/item/manager/ItemManager.h>
#include <application/object/character/enemy/manager/EnemyManager.h>

using namespace Norm;

void ObjectSystem::Initialize() {
}

void ObjectSystem::Update() {
	assert(player_ && "プレイヤーがセットされていません");
	assert(itemManager_ && "アイテムマネージャーがセットされていません");
	assert(enemyManager_ && "エネミーマネージャーがセットされていません");
}

void ObjectSystem::SpawnItem(const Norm::Vector3& pos) {
	//スポーン
	itemManager_->SpawnTutorialCollectibleItme(pos);
}

void ObjectSystem::SpawnEnemy(const std::string& enemyKind, const Norm::Vector3& pos, const Norm::Vector3& rotate, bool isActive) {
	if (enemyKind == "Canota") {
		enemyManager_->CanotaSpawn(pos, rotate);
	}
	else if (enemyKind == "KeyCanota") {
		enemyManager_->KeyCanotaSpawn(pos, rotate);
	}
	else if (enemyKind == "Jet") {
		enemyManager_->JetSpawn(pos, rotate);
	}
	else if (enemyKind == "Boss") {
		enemyManager_->BossSpawn(pos, rotate);
	}
}

void ObjectSystem::FixPlayer(const Norm::Vector3& pos, const Norm::Vector3& rot) {
	//プレイヤーの位置を固定
	player_->SetTranslate(pos);
	player_->SetRotate(rot);
}
