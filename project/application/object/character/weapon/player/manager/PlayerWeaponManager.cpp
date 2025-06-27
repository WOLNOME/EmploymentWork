#include "PlayerWeaponManager.h"

//アプリケーション
#include "application/object/character/player/Player.h"

void PlayerWeaponManager::Initialize() {
	//パラメーターの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerWeaponManager");

	//砲弾の生成と初期化
	int cannonNum = param_["maxCannonNum"];
	for (int i = 0; i < cannonNum; i++) {
		cannons_.emplace_back(std::make_unique<PlayerCannon>());
		cannons_[i]->Initialize();
	}

	//銃弾の生成と初期化
	int bulletNum = param_["maxBulletNum"];
	for (int i = 0; i < bulletNum; i++) {
		bullets_.emplace_back(std::make_unique<PlayerBullet>());
		bullets_[i]->Initialize();
	}
}

void PlayerWeaponManager::Update() {
	//砲弾の生成
	CreateCannon();
	//銃弾の生成
	CreateBullet();

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

void PlayerWeaponManager::CreateCannon() {
	//プレイヤーから発射フラグを取得
	if (!player_->GetIsCannonFire()) return;
	//砲弾の追加位置を探す
	for (auto& cannon : cannons_) {
		//砲弾が生きていたら次へ
		if (!cannon->GetIsDead()) continue;
		//砲弾の初期位置と初速度をセット
		float orx = camera_->worldTransform.rotate.x;
		float ory = camera_->worldTransform.rotate.y;
		Vector3 currentDir = {
			std::cosf(orx) * std::sinf(ory),
			-std::sinf(orx),		//←角度
			std::cosf(orx) * std::cosf(ory)
		};
		currentDir.Normalize();
		Vector3 cannonPos = player_->GetWorldTransform().translate;
		cannonPos.y += 1.7f;	//砲弾の初期位置を調整
		Vector3 cannonDirection = currentDir;
		cannon->SetInitParam(cannonPos, cannonDirection);
		//カメラシェイクを入れる
		camera_->RegistShake(0.2f, 0.15f);
		break;
	}
	
}

void PlayerWeaponManager::CreateBullet() {
	//プレイヤーから発射フラグを取得
	if (!player_->GetIsBulletFire()) return;
	//銃弾の追加位置を探す
	for (auto& bullet : bullets_) {
		//銃弾が生きていたら次へ
		if (!bullet->GetIsDead()) continue;
		//銃弾の初期位置と初速度をセット
		float orx = camera_->worldTransform.rotate.x;
		float ory = camera_->worldTransform.rotate.y;
		Vector3 currentDir = {
			std::cosf(orx) * std::sinf(ory),
			-std::sinf(orx),		//←角度
			std::cosf(orx) * std::cosf(ory)
		};
		currentDir.Normalize();
		Vector3 bulletPos = camera_->worldTransform.translate;
		bulletPos += currentDir * 8.0f;	//銃弾の初期位置を調整
		bullet->SetInitParam(bulletPos, currentDir);
		break;
	}
}
