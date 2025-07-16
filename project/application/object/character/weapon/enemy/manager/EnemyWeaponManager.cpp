#include "EnemyWeaponManager.h"

//アプリケーション
#include "application/object/character/enemy/manager/EnemyManager.h"
#include "application/object/character/player/Player.h"
#include <application/object/character/enemy/base/IBaseEnemy.h>

void EnemyWeaponManager::Initialize() {
	//パラメーターの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/enemyWeaponManager");

	//砲弾の生成と初期化
	int cannonNum = param_["maxCannonNum"];
	for (int i = 0; i < cannonNum; i++) {
		cannons_.emplace_back(std::make_unique<EnemyCannon>());
		cannons_[i]->Initialize();
	}

}

void EnemyWeaponManager::Update() {
	//砲弾の生成
	CreateCannon();

	//砲弾の更新
	for (auto& cannon : cannons_) {
		cannon->Update();
	}
}

void EnemyWeaponManager::DebugWithImGui() {
#ifdef _DEBUG
	//砲弾のデバッグ
	for (const auto& cannon : cannons_) {
		cannon->DebugWithImGui();
	}
#endif // _DEBUG
}

void EnemyWeaponManager::SetPlayerUI(PlayerUI* _playerUI) {
	//砲弾全てに渡す
	for (auto& cannon : cannons_) {
		cannon->SetPlayerUI(_playerUI);
	}
}

void EnemyWeaponManager::CreateCannon() {
	//全てのエネミーを回す
	for (auto& canota : enemyManager_->GetCanotas()) {
		//エネミーから発射フラグを取得
		if (!canota->GetAttackState()->GetIsCannonFire()) continue;
		//砲弾の追加位置を探す
		for (auto& cannon : cannons_) {
			//砲弾が生きていたら次へ
			if (!cannon->GetIsDead()) continue;
			//砲弾の初期位置と目標位置をセット
			Vector3 cannonPos = canota->GetWorldTransform().translate;
			cannonPos.y += 2.0f;	//←高さ
			cannonPos.x += std::sinf(canota->GetWorldTransform().rotate.y) * 10.0f;
			cannonPos.z += std::cosf(canota->GetWorldTransform().rotate.y) * 10.0f;
			cannon->SetInitParam(cannonPos, player_->GetWorldTransform().translate);
			break;
		}
	}

	//全てのボスを回す
	for (auto& boss : enemyManager_->GetBosses()) {
		//ボスから発射フラグを取得
		if (!boss->GetAttackState()->GetIsCannonFire()) continue;
		//砲弾の追加位置を探す
		for (auto& cannon : cannons_) {
			//砲弾が生きていたら次へ
			if (!cannon->GetIsDead()) continue;
			//砲弾の初期位置と目標位置をセット
			Vector3 cannonPos = boss->GetWorldTransform().translate;
			cannonPos.y += 2.0f;	//←高さ
			cannonPos.x += std::sinf(boss->GetWorldTransform().rotate.y) * 10.0f;
			cannonPos.z += std::cosf(boss->GetWorldTransform().rotate.y) * 10.0f;
			cannon->SetInitParam(cannonPos, player_->GetWorldTransform().translate);
			break;
		}
	}
}
