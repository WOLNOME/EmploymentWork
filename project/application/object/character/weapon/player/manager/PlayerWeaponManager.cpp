#include "PlayerWeaponManager.h"

//アプリケーション
#include "application/object/character/player/Player.h"

void PlayerWeaponManager::Initialize() {
	//パラメーターの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerWeaponManager");

	//砲弾の生成と初期化
	int cannonNum = param_["maxCannonNum"];
	for (int i = 0; i < cannonNum; i++) {
		cannons_.push_back(std::make_unique<PlayerCannon>());
		cannons_[i]->Initialize();
	}

	//銃弾の生成と初期化
	int bulletNum = param_["maxBulletNum"];
	for (int i = 0; i < bulletNum; i++) {
		bullets_.push_back(std::make_unique<PlayerBullet>());
		bullets_[i]->Initialize();
	}
}

void PlayerWeaponManager::Update() {
	//砲弾の更新
	for (auto& cannon : cannons_) {
		cannon->Update();
	}
	//銃弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
	}
}

void PlayerWeaponManager::DebugWithImGui() {
#ifdef _DEBUG
	//砲弾のデバッグ
	for (const auto& cannon : cannons_) {
		cannon->DebugWithImGui();
	}
	//銃弾のデバッグ
	for (const auto& bullet : bullets_) {
		bullet->DebugWithImGui();
	}
#endif // _DEBUG
}

void PlayerWeaponManager::SpawnCannon(const Vector3& _initPos, const Vector3& _initDirection) {
	//砲弾のコンテナを走査
	for (auto& cannon : cannons_) {
		//砲弾がアイドル状態でないなら次へ
		if (cannon->GetState() != BaseCharacter::State::kIdle)
			continue;
		//スポーン
		cannon->Spawn(_initPos, _initDirection);
		//カメラシェイクを入れる
		camera_->RegistShake(0.2f, 0.15f);

		break;
	}
}

void PlayerWeaponManager::SpawnBullet(const Vector3& _initPos, const Vector3& _initDirection) {
	//銃弾のコンテナを走査
	for (auto& bullet : bullets_) {
		//銃弾がアイドル状態でないなら次へ
		if (bullet->GetState() != BaseCharacter::State::kIdle)
			continue;
		//スポーン
		bullet->Spawn(_initPos, _initDirection);

		break;
	}
}
