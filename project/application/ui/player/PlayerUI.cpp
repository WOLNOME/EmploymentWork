#include "PlayerUI.h"
#include <WinApp.h>
#include <TextureManager.h>
#include <SpriteManager.h>
#include <GameCamera.h>

#include <random>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>
#include <application/object/character/enemy/manager/EnemyManager.h>

void PlayerUI::Initialize() {
	//パラメータ読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerUI");

	//メンバ変数の生成と初期化
	{
		//2dレティクルの初期化
		th2dReticle_ = TextureManager::GetInstance()->LoadTexture("reticle.png");
		sprite2dReticle_ = std::make_unique<Sprite>();
		sprite2dReticle_->Initialize(SpriteManager::GetInstance()->GenerateName("Player2dReticle"), Sprite::Order::Front0, th2dReticle_);
		sprite2dReticle_->SetAnchorPoint({ 0.5f,0.5f });
		sprite2dReticle_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
	}
	{
		//FPSUIの初期化
		thFPSUI_ = TextureManager::GetInstance()->LoadTexture("FPSUI.png");
		spriteFPSUI_ = std::make_unique<Sprite>();
		spriteFPSUI_->Initialize(SpriteManager::GetInstance()->GenerateName("PlayerFPSUI"), Sprite::Order::Front0, thFPSUI_);
		spriteFPSUI_->SetAnchorPoint({ 0.5f,0.5f });
		spriteFPSUI_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
	}
	{
		//HPバーの初期化
		thHPBar_[0] = TextureManager::GetInstance()->LoadTexture("hp_redBar.png");
		thHPBar_[1] = TextureManager::GetInstance()->LoadTexture("hp_greenBar.png");
		for (int i = 0; i < thHPBar_.size(); i++) {
			spriteHPBar_[i] = std::make_unique<Sprite>();
			if (i == 0)
				spriteHPBar_[i]->Initialize(SpriteManager::GetInstance()->GenerateName("PlayerHPBar"), Sprite::Order::Front1, thHPBar_[i]);
			else
				spriteHPBar_[i]->Initialize(SpriteManager::GetInstance()->GenerateName("PlayerHPBar"), Sprite::Order::Front2, thHPBar_[i]);
			spriteHPBar_[i]->SetPosition({ 320.0f,20.0f });
		}
	}
	{
		//砲弾UIの初期化
		thCannon_[0] = TextureManager::GetInstance()->LoadTexture("cannonUI.png");
		thCannon_[1] = TextureManager::GetInstance()->LoadTexture("black.png");
		for (int i = 0; i < thCannon_.size(); i++) {
			spriteCannon_[i] = std::make_unique<Sprite>();
			if (i == 0)
				spriteCannon_[i]->Initialize(SpriteManager::GetInstance()->GenerateName("PlayerCannonUI"), Sprite::Order::Front1, thCannon_[i]);
			else
				spriteCannon_[i]->Initialize(SpriteManager::GetInstance()->GenerateName("PlayerCannonUI"), Sprite::Order::Front2, thCannon_[i]);
			spriteCannon_[i]->SetPosition({ 330.0f,565.0f });
		}
		spriteCannon_[1]->SetSize({ spriteCannon_[0]->GetSize() });
	}
	{
		//銃弾UIの初期化
		thBullet_[0] = TextureManager::GetInstance()->LoadTexture("bulletUI.png");
		thBullet_[1] = TextureManager::GetInstance()->LoadTexture("black.png");
		for (int i = 0; i < thBullet_.size(); i++) {
			spriteBullet_[i] = std::make_unique<Sprite>();
			if (i == 0)
				spriteBullet_[i]->Initialize(SpriteManager::GetInstance()->GenerateName("PlayerBulletUI"), Sprite::Order::Front1, thBullet_[i]);
			else
				spriteBullet_[i]->Initialize(SpriteManager::GetInstance()->GenerateName("PlayerBulletUI"), Sprite::Order::Front2, thBullet_[i]);
			spriteBullet_[i]->SetPosition({ 830.0f,565.0f });
		}
		spriteBullet_[1]->SetSize({ spriteBullet_[0]->GetSize() });
	}
	{
		//レーダーUIの初期化
		radar_ = std::make_unique<Radar>();
		radar_->Initialize();
	}
}

void PlayerUI::Update() {
	//playerが読み込まれていなかったらassert
	assert(player_ != nullptr && "PlayerUIにPlayerインスタンスを渡してください");
	//enemyManagerが読み込まれていなかったらassert
	assert(enemyManager_ != nullptr && "PlayerUIにEnemyManagerインスタンスを渡してください");

	//レーダーUIの更新
	radar_->Update();

	//緑HPバーのサイズをプレイヤーのHPに合わせる
	float hpRate = (float)player_->GetHP() / (float)player_->GetMaxHP();
	spriteHPBar_[1]->SetSize({ spriteHPBar_[0]->GetSize().x * hpRate,spriteHPBar_[0]->GetSize().y });

	//砲弾UIのマスクを砲弾クールタイムと同期させる
	float cannonBallCoolRate = player_->GetCannonReloadTimer() / player_->GetCannonReloadTime();
	spriteCannon_[1]->SetSize({ spriteCannon_[0]->GetSize().x, spriteCannon_[0]->GetSize().y * cannonBallCoolRate });

	//銃弾UIのマスクの処理
	if (player_->GetBulletReloadTimer() > 0.0f) {
		//銃弾リロードタイムと同期させる
		float bulletReloadRate = player_->GetBulletReloadTimer() / player_->GetBulletReloadTime();
		spriteBullet_[1]->SetSize({ spriteBullet_[0]->GetSize().x, spriteBullet_[0]->GetSize().y * bulletReloadRate });
	}
	else {
		//残弾数に合わせる
		float bulletNumRate = (float)player_->GetBulletNum() / (float)player_->GetBulletMaxNum();
		spriteBullet_[1]->SetSize({ spriteBullet_[0]->GetSize().x, spriteBullet_[0]->GetSize().y * (1.0f - bulletNumRate) });
	}

	//もしカメラが揺れてたらUIも一部揺らす(オフセットはそろえる)
	if (camera_->GetIsShake()) {
		std::random_device seed_gen;
		std::mt19937 engine(seed_gen());
		int shakePower = param_["shakePower"];
		std::uniform_int_distribution<int> dist(int(-shakePower * camera_->GetShakePower()), int(shakePower * camera_->GetShakePower()));
		//オフセット
		Vector2 offset = { (float)dist(engine),(float)dist(engine) };
		//設定
		spriteFPSUI_->SetShakeOffset(offset);
		for (int i = 0; i < 2; i++) {
			spriteHPBar_[i]->SetShakeOffset(offset);
			spriteCannon_[i]->SetShakeOffset(offset);
			spriteBullet_[i]->SetShakeOffset(offset);
		}
		radar_->AttachShake(offset);
	}

	//ダメージによる点滅処理
	DamageBlinking();

}

void PlayerUI::DebugWithImGui() {
#ifdef _DEBUG

#endif //_DEBUG
}

void PlayerUI::SetPlayer(Player* _player) {
	player_ = _player;
	//レーダーUIにも渡す
	radar_->SetPlayer(player_);
}

void PlayerUI::SetEnemyManager(EnemyManager* _enemyManager) {
	enemyManager_ = _enemyManager;
	//レーダーUIにも渡す
	radar_->SetEnemyManager(enemyManager_);
}

void PlayerUI::SetCamera(GameCamera* _camera) {
	camera_ = _camera;
	//レーダーUIにも渡す
	radar_->SetCamera(camera_);
}

void PlayerUI::DamageBlinking() {
	//共通関数：すべてのスプライトに色をセット
	auto SetUIColor = [&](const Vector4& color) {
		spriteFPSUI_->SetColor(color);
		for (int i = 0; i < 2; ++i) {
			spriteHPBar_[i]->SetColor(color);
			spriteCannon_[i]->SetColor(color);
			spriteBullet_[i]->SetColor(color);
		}
		//レーダーUIにも色の変化を適用
		radar_->AttachBlinking(color);
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
