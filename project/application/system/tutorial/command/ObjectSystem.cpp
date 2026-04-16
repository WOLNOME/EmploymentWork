#include "ObjectSystem.h"
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>
#include <application/object/character/item/manager/ItemManager.h>
#include <application/object/character/enemy/manager/EnemyManager.h>
#include <application/system/CameraManager.h>

using namespace Norm;

void ObjectSystem::Initialize() {
}

void ObjectSystem::Update() {
	assert(player_ && "プレイヤーがセットされていません");
	assert(itemManager_ && "アイテムマネージャーがセットされていません");
	assert(enemyManager_ && "エネミーマネージャーがセットされていません");
}

void ObjectSystem::SpawnItem(const std::string& itemKind, const Norm::Vector3& pos) {

	if (itemKind == "Item") {
		itemManager_->SpawnTutorialCollectibleItme(pos);
	}
	else if (itemKind == "Special") {
		itemManager_->SpawnChargeItem(pos);
	}
	
}

void ObjectSystem::SpawnEnemy(const std::string& enemyKind, const Norm::Vector3& pos, const Norm::Vector3& rotate) {
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

void ObjectSystem::ClearEnemy() {
	enemyManager_->ClearEnemy();
}

void ObjectSystem::SetIsActiveEnemy(bool isActive) {
	enemyManager_->SetIsActive(isActive);
}

void ObjectSystem::SetIsInvicibleEnemy(bool isInvicible) {
	enemyManager_->SetIsInvicible(isInvicible);
}

void ObjectSystem::FixPlayer(const Norm::Vector3& pos, const Norm::Vector3& rot) {
	//プレイヤーの位置を固定
	player_->SetTranslate(pos);
	player_->SetRotate(rot);
	//カメラの回転をプレイヤーの向きに合わせる
	player_->GetCameraManager()->GetActiveCamera()->worldTransform.SetRotate(rot);
}
