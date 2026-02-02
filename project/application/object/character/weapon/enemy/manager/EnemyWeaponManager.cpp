#include "EnemyWeaponManager.h"

//アプリケーション
#include "application/object/character/enemy/manager/EnemyManager.h"
#include "application/object/character/player/Player.h"
#include <application/object/character/enemy/tank/base/IBaseTankEnemy.h>

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

void EnemyWeaponManager::SpawnCannon(const Vector3& _initPos, const Vector3& _targetPos) {
	//砲弾のコンテナを走査
	for (auto& cannon : cannons_) {
		//砲弾がアイドル状態でないなら次へ
		if (cannon->GetState() != BaseCharacter::State::kIdle)
			continue;
		//スポーン
		cannon->Spawn(_initPos, _targetPos);

		break;
	}
}

void EnemyWeaponManager::SpawnBullet(const Vector3& _initPos, const Vector3& _targetPos) {
	//銃弾のコンテナを走査
	for (auto& bullet : bullets_) {
		//銃弾がアイドル状態でないなら次へ
		if (bullet->GetState() != BaseCharacter::State::kIdle)
			continue;
		//スポーン
		bullet->Spawn(_initPos, _targetPos);

		break;
	}
}

void EnemyWeaponManager::SpawnBomb(const BombMethod& _bombMethod, const Vector3& _initPos, const Vector3& _targetPos) {
	//爆弾の追加位置を探す
	for (auto& bomb : bombs_) {
		//爆弾がアイドル状態でないなら次へ
		if (bomb->GetState() != BaseCharacter::State::kIdle)
			continue;
		//スポーン
		bomb->Spawn(_initPos, _targetPos);

		break;
	}
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