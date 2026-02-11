#include "PlayerWeaponManager.h"

//アプリケーション
#include "application/object/character/player/Player.h"

using namespace Norm;

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

	//必殺弾の生成と初期化
	int specialNum = param_["maxSpecialNum"];
	for (int i = 0; i < specialNum; i++) {
		specials_.push_back(std::make_unique<PlayerSpecial>());
		specials_[i]->Initialize();
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
	//必殺弾の更新
	for (auto& special : specials_) {
		special->Update();
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
	//必殺弾のデバッグ
	for (const auto& special : specials_) {
		special->DebugWithImGui();
	}
#endif // _DEBUG
}

void PlayerWeaponManager::SpawnCannon(const Vector3& _initPos, const Vector3& _initDirection) {
	//スポーン
	SpawnFromPool(cannons_, [&](PlayerCannon* cannon) {
		cannon->Spawn(_initPos, _initDirection);
		camera_->RegistShake(0.2f, 0.15f);
		});
}

void PlayerWeaponManager::SpawnBullet(const Vector3& _initPos, const Vector3& _initDirection) {
	//スポーン
	SpawnFromPool(bullets_, [&](PlayerBullet* bullet) {
		bullet->Spawn(_initPos, _initDirection);
		camera_->RegistShake(0.15f, 0.1f);
		});
}

void PlayerWeaponManager::SpawnSpecial(const Norm::Vector3& _initPos, const Norm::Vector3& _initDirection) {
	//スポーン
	SpawnFromPool(specials_, [&](PlayerSpecial* special) {
		special->Spawn(_initPos, _initDirection);
		camera_->RegistShake(0.4f, 0.3f);
		});
}
