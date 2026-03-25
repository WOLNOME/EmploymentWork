#include "ObjectSystem.h"
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>
#include <application/object/character/item/manager/ItemManager.h>

using namespace Norm;

void ObjectSystem::Initialize() {
}

void ObjectSystem::Update() {
	assert(player_ && "プレイヤーがセットされていません");
	assert(itemManager_ && "アイテムマネージャーがセットされていません");
}

void ObjectSystem::SpawnItem(const Norm::Vector3& pos) {
	//スポーン
	itemManager_->SpawnTutorialCollectibleItme(pos);
}

void ObjectSystem::FixPlayer(const Norm::Vector3& pos, const Norm::Vector3& rot) {
	//プレイヤーの位置を固定
	player_->SetTranslate(pos);
	player_->SetRotate(rot);
}
