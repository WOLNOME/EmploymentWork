#include "PlayerUI.h"
#include <GameCamera.h>
#include <random>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>
#include <application/object/character/enemy/manager/EnemyManager.h>

using namespace Norm;

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

	//武器UI
	weaponUI_ = std::make_unique<WeaponUI>();
	weaponUI_->Initialize();

	//レーダーUI
	radar_ = std::make_unique<Radar>();
	radar_->Initialize();

	//被弾インジケーターUI
	hitIndicator_ = std::make_unique<HitIndicator>();
	hitIndicator_->Initialize();

	//アイテムUI
	itemUI_ = std::make_unique<ItemUI>();
	itemUI_->Initialize();

	//操作UI
	operationUI_ = std::make_unique<OperationUI>();
	operationUI_->Initialize();

	//移動レバーUI
	moveLeverUI_ = std::make_unique<MoveLever>();
	moveLeverUI_->Initialize();

}

void PlayerUI::Update() {
	//playerが読み込まれていなかったらassert
	assert(player_ != nullptr && "PlayerUIにPlayerインスタンスを渡してください");

	//装飾用UIの更新
	decorativeUI_->Update();
	//プレイヤーHPUIの更新
	playerHPUI_->Update();
	//武器UIの更新
	weaponUI_->Update();
	//レーダーUIの更新
	radar_->Update();
	//被弾インジケーターUIの更新
	hitIndicator_->Update();
	//アイテムUIの更新
	itemUI_->Update();
	//操作UIの更新
	operationUI_->Update();
	//移動レバーUIの更新
	moveLeverUI_->Update();

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
	//装飾用UIに渡す
	decorativeUI_->SetPlayer(_player);
	//プレイヤーHPUIに渡す
	playerHPUI_->SetPlayer(player_);
	//武器UIに渡す
	weaponUI_->SetPlayer(player_);
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
		weaponUI_->AttachShake(offset);
		radar_->AttachShake(offset);
		itemUI_->AttachShake(offset);
		operationUI_->AttachShake(offset);
		moveLeverUI_->AttachShake(offset);
	}
}

void PlayerUI::DamageBlinking() {
	//共通関数：すべてのスプライトに色をセット
	auto SetUIColor = [&](const Vector4& color) {
		//装飾用UI
		decorativeUI_->AttachBlinking(color);
		//HPUI
		playerHPUI_->AttachBlinking(color);
		//武器UI
		weaponUI_->AttachBlinking(color);
		//レーダーUI
		radar_->AttachBlinking(color);
		//アイテムUI
		itemUI_->AttachBlinking(color);
		//操作UI
		operationUI_->AttachBlinking(color);
		//移動レバーUI
		moveLeverUI_->AttachBlinking(color);

		};

	//被弾開始時または死亡演出中に点滅開始
	if ((player_->GetIsDamage() || player_->GetIsDeathDir()) && blinkTimer_ <= 0.0f) {
		float blinkDuration = param_["blinkDuration"];
		blinkTimer_ = blinkDuration;
		isDamage_ = true;
	}

	//点滅処理
	if (isDamage_) {
		blinkTimer_ -= kDeltaTime;

		//点滅トグル
		static float blinkElapsed = 0.0f;
		blinkElapsed += kDeltaTime;

		float blinkInterval = param_["blinkInterval"];
		if (blinkElapsed >= blinkInterval) {
			blinkElapsed = 0.0f;
			isBright_ = !isBright_;

			Vector4 color = isBright_ ? Vector4{ 1.0f, 0.6f, 0.6f, 1.0f }
			: Vector4{ 0.6f, 0.2f, 0.2f, 1.0f };
			SetUIColor(color);
		}

		//終了処理
		if (blinkTimer_ <= 0.0f) {
			SetUIColor({ 1.0f, 1.0f, 1.0f, 1.0f }); //元の色に戻す
			isDamage_ = false;
			isBright_ = true;
			blinkElapsed = 0.0f;
			blinkTimer_ = 0.0f;
		}
	}
}
