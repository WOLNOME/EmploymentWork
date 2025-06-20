#include "EnemyWeaponManager.h"

//アプリケーション
#include "application/EnemyManager.h"
#include "application/object/player/Player.h"

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

void EnemyWeaponManager::Draw() {
	//砲弾の描画
	for (const auto& cannon : cannons_) {
		cannon->Draw();
	}
}

void EnemyWeaponManager::DrawLine() {
	//砲弾のライン描画
	for (const auto& cannon : cannons_) {
		cannon->DrawLine();
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

void EnemyWeaponManager::SetCamera(GameCamera* _camera) {
	camera_ = _camera;
	for (auto& cannon : cannons_) {
		cannon->SetCamera(_camera);
	}
}

void EnemyWeaponManager::SetLight(SceneLight* _light) {
	sceneLight_ = _light;
	for (auto& cannon : cannons_) {
		cannon->SetSceneLight(_light);
	}
}

void EnemyWeaponManager::CreateCannon() {
	//全てのエネミーを回す
	for (auto& enemy : enemyManager_->GetEnemies()) {
		//エネミーから発射フラグを取得
		if (!enemy->GetIsCannonFire()) continue; 
		//砲弾の追加位置を探す
		for (auto& cannon : cannons_) {
			//砲弾が生きていたら次へ
			if (!cannon->GetIsDead()) continue;
			//砲弾の初期位置と目標位置をセット
			Vector3 cannonPos = enemy->GetWorldTransform().translate;
			cannonPos.y += 2.0f;	//←高さ
			cannonPos.x += std::sinf(enemy->GetWorldTransform().rotate.y) * 10.0f;
			cannonPos.z += std::cosf(enemy->GetWorldTransform().rotate.y) * 10.0f;
			cannon->SetInitParam(cannonPos, player_->GetWorldTransform().translate);
			break;
		}
	}
	
	//ボスから発射フラグを取得
	if (!enemyManager_->GetBoss()->GetIsCannonFire()) return;
	//砲弾の追加位置を探す
	for (auto& cannon : cannons_) {
		//砲弾が生きていたら次へ
		if (!cannon->GetIsDead()) continue;
		//砲弾の初期位置と目標位置をセット
		Vector3 cannonPos = enemyManager_->GetBoss()->GetWorldTransform().translate;
		cannonPos.y += 2.0f;	//←高さ
		cannonPos.x += std::sinf(enemyManager_->GetBoss()->GetWorldTransform().rotate.y) * 10.0f;
		cannonPos.z += std::cosf(enemyManager_->GetBoss()->GetWorldTransform().rotate.y) * 10.0f;
		cannon->SetInitParam(cannonPos, player_->GetWorldTransform().translate);
		break;
	}

}
