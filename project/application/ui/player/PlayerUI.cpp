#include "PlayerUI.h"
#include <GameCamera.h>
#include <random>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>
#include <application/object/character/enemy/manager/EnemyManager.h>

void PlayerUI::Initialize() {
	//パラメータ読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerUI");

	//レティクルUI
	reticleUI_ = std::make_unique<ReticleUI>();
	reticleUI_->Initialize();

	//装飾用UI
	decorativeUI_ = std::make_unique<DecorativeUI>();
	decorativeUI_->Initialize();

	//HPバー
	playerHPUI_ = std::make_unique<PlayerHPUI>();
	playerHPUI_->Initialize();

	//砲弾UI
	cannonUI_ = std::make_unique<CannonUI>();
	cannonUI_->Initialize();

	//銃弾UI
	bulletUI_ = std::make_unique<BulletUI>();
	bulletUI_->Initialize();

	//レーダーUI
	radar_ = std::make_unique<Radar>();
	radar_->Initialize();

	//被弾インジケーターUI
	hitIndicator_ = std::make_unique<HitIndicator>();
	hitIndicator_->Initialize();

	//アイテムUI
	itemUI_ = std::make_unique<ItemUI>();
	itemUI_->Initialize();
}

void PlayerUI::Update() {
	//playerが読み込まれていなかったらassert
	assert(player_ != nullptr && "PlayerUIにPlayerインスタンスを渡してください");

	//プレイヤーHPUIの更新
	playerHPUI_->Update();
	//砲弾UIの更新
	cannonUI_->Update();
	//弾丸UIの更新
	bulletUI_->Update();
	//レーダーUIの更新
	radar_->Update();
	//被弾インジケーターUIの更新
	hitIndicator_->Update();
	//アイテムUIの更新
	itemUI_->Update();

	//ダメージによるシェイク処理
	DamageShaking();

	//ダメージによる点滅処理
	DamageBlinking();

}

void PlayerUI::DebugWithImGui() {
#ifdef _DEBUG

#endif //_DEBUG
}

void PlayerUI::SetPlayer(Player* _player) {
	player_ = _player;
	//プレイヤーHPUIに渡す
	playerHPUI_->SetPlayer(player_);
	//砲弾UIに渡す
	cannonUI_->SetPlayer(player_);
	//弾丸UIに渡す
	bulletUI_->SetPlayer(player_);
	//レーダーUIに渡す
	radar_->SetPlayer(player_);
	//被弾インジケーターUIに渡す
	hitIndicator_->SetPlayer(player_);
	//アイテムUIに渡す
	itemUI_->SetPlayer(player_);
}

void PlayerUI::SetEnemyManager(EnemyManager* _enemyManager) {
	//レーダーUIに渡す
	radar_->SetEnemyManager(_enemyManager);
}

void PlayerUI::SetItemManager(ItemManager* _itemManager) {
	//レーダーUIに渡す
	radar_->SetItemManager(_itemManager);
}

void PlayerUI::SetGameCamera(GameCamera* _camera) {
	camera_ = _camera;
	//レーダーUIに渡す
	radar_->SetGameCamera(camera_);
	//被弾インジケーターUIに渡す
	hitIndicator_->SetGameCamera(camera_);
}

void PlayerUI::DamageShaking() {
	//もしカメラが揺れてたらUIも一部揺らす(オフセットはそろえる)
	if (camera_->GetIsShake()) {
		std::random_device seed_gen;
		std::mt19937 engine(seed_gen());
		int shakePower = param_["shakePower"];
		std::uniform_int_distribution<int> dist(int(-shakePower * camera_->GetShakePower()), int(shakePower * camera_->GetShakePower()));
		//オフセット
		Vector2 offset = { (float)dist(engine),(float)dist(engine) };
		//設定
		decorativeUI_->AttachShake(offset);
		playerHPUI_->AttachShake(offset);
		cannonUI_->AttachShake(offset);
		bulletUI_->AttachShake(offset);
		radar_->AttachShake(offset);
		itemUI_->AttachShake(offset);
	}
}

void PlayerUI::DamageBlinking() {
	//共通関数：すべてのスプライトに色をセット
	auto SetUIColor = [&](const Vector4& color) {
		//装飾用UI
		decorativeUI_->AttachBlinking(color);
		//HPUI
		playerHPUI_->AttachBlinking(color);
		//砲弾UI
		cannonUI_->AttachBlinking(color);
		//弾丸UI
		bulletUI_->AttachBlinking(color);
		//レーダーUI
		radar_->AttachBlinking(color);
		//アイテムUI
		itemUI_->AttachBlinking(color);
		};

	//被弾開始時に点滅開始
	if (player_->GetIsDamage() && blinkTimer <= 0.0f) {
		float blinkDuration = param_["blinkDuration"];
		blinkTimer = blinkDuration;
		isDamage = true;
	}

	//点滅処理
	if (isDamage) {
		blinkTimer -= kDeltaTime;

		//点滅トグル
		static float blinkElapsed = 0.0f;
		blinkElapsed += kDeltaTime;

		float blinkInterval = param_["blinkInterval"];
		if (blinkElapsed >= blinkInterval) {
			blinkElapsed = 0.0f;
			isBright = !isBright;

			Vector4 color = isBright ? Vector4{ 1.0f, 0.6f, 0.6f, 1.0f }
			: Vector4{ 0.6f, 0.2f, 0.2f, 1.0f };
			SetUIColor(color);
		}

		//終了処理
		if (blinkTimer <= 0.0f) {
			SetUIColor({ 1.0f, 1.0f, 1.0f, 1.0f }); //元の色に戻す
			isDamage = false;
			isBright = true;
			blinkElapsed = 0.0f;
			blinkTimer = 0.0f;
		}
	}
}
