#include "EnemyWeaponManager.h"

//アプリケーション
#include "application/object/character/enemy/manager/EnemyManager.h"
#include "application/object/character/player/Player.h"
#include <application/object/character/enemy/tank/base/IBaseTankEnemy.h>
#include <application/system/CameraManager.h>

using namespace Norm;

void EnemyWeaponManager::Initialize() {
	//パラメーターの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/enemyWeaponManager");

	//砲弾の生成と初期化
	int cannonNum = param_["maxCannonNum"];
	for (int i = 0; i < cannonNum; i++) {
		cannons_.push_back(std::make_unique<EnemyCannon>());
		cannons_[i]->Initialize();
	}
	//銃弾の生成と初期化
	int bulletNum = param_["maxBulletNum"];
	for (int i = 0; i < bulletNum; i++) {
		bullets_.push_back(std::make_unique<EnemyBullet>());
		bullets_[i]->Initialize();
	}
	//爆弾の生成と初期化
	int bombNum = param_["maxBombNum"];
	for (int i = 0; i < bombNum; i++) {
		bombs_.push_back(std::make_unique<EnemyBomb>());
		bombs_[i]->Initialize();
	}

}

void EnemyWeaponManager::Update() {
	//砲弾の更新
	for (auto& cannon : cannons_) {
		cannon->Update();
	}
	//銃弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
	}
	//爆弾の更新
	for (auto& bomb : bombs_) {
		bomb->Update();
	}
}

void EnemyWeaponManager::DebugWithImGui() {
#ifdef _DEBUG
	//砲弾のデバッグ
	for (const auto& cannon : cannons_) {
		cannon->DebugWithImGui();
	}
	//銃弾のデバッグ
	for (const auto& bullet : bullets_) {
		bullet->DebugWithImGui();
	}
	//爆弾のデバッグ
	for (const auto& bomb : bombs_) {
		bomb->DebugWithImGui();
	}
#endif // _DEBUG
}

void EnemyWeaponManager::SpawnCannon(const Vector3& _initPos, const Vector3& _targetPos, float _size) {
	//スポーン
	SpawnFromPool(cannons_, [&](EnemyCannon* cannon) {
		cannon->Spawn(_initPos, _targetPos, _size);
		});
}

void EnemyWeaponManager::SpawnBullet(const Vector3& _initPos, const Vector3& _targetPos) {
	//スポーン
	SpawnFromPool(bullets_, [&](EnemyBullet* bullet) {
		bullet->Spawn(_initPos, _targetPos);
		});
}

void EnemyWeaponManager::SpawnBomb(const BombMethod& _bombMethod, const Vector3& _initPos, const Vector3& _targetPos, float _size) {
	//スポーン
	SpawnFromPool(bombs_, [&](EnemyBomb* bomb) {
		bomb->Spawn(_bombMethod, _initPos, _targetPos, _size);
		});
}

void EnemyWeaponManager::SetPlayerUI(PlayerUI* _playerUI) {
	//砲弾全てに渡す
	for (auto& cannon : cannons_) {
		cannon->SetPlayerUI(_playerUI);
	}
	//銃弾全てに渡す
	for (auto& bullet : bullets_) {
		bullet->SetPlayerUI(_playerUI);
	}
	//爆弾全てに渡す
	for (auto& bomb : bombs_) {
		bomb->SetPlayerUI(_playerUI);
	}
}

void EnemyWeaponManager::SetCameraManager(CameraManager* _cameraManager) {
	//砲弾全てに渡す
	for (auto& cannon : cannons_) {
		cannon->SetCameraManager(_cameraManager);
	}
	//銃弾全てに渡す
	for (auto& bullet : bullets_) {
		bullet->SetCameraManager(_cameraManager);
	}
	//爆弾全てに渡す
	for (auto& bomb : bombs_) {
		bomb->SetCameraManager(_cameraManager);
	}
}