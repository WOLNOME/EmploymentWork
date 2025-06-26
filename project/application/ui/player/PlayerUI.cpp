#include "PlayerUI.h"
#include "WinApp.h"
#include "TextureManager.h"
#include "SpriteManager.h"
#include <random>
#include <cassert>

//アプリケーション
#include "application/object/character/player/Player.h"

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
		spriteCannon_[1]->SetColor({ 0.0f,0.0f,0.0f,0.94f });
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
		spriteBullet_[1]->SetColor({ 0.0f,0.0f,0.0f,0.94f });
	}


}

void PlayerUI::Update() {
	//playerが読み込まれていなかったらassert
	assert(player_ != nullptr && "PlayerUIにPlayerインスタンスを渡してください");

	//HPバーのサイズをプレイヤーのHPに合わせる
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
		std::uniform_int_distribution<int> dist(-shakePower * (int)camera_->GetShakePower(), shakePower * (int)camera_->GetShakePower());
		//オフセット
		Vector2 offset = { (float)dist(engine),(float)dist(engine) };

		//設定
		spriteFPSUI_->SetShakeOffset(offset);
		for (int i = 0; i < 2; i++) {
			spriteHPBar_[i]->SetShakeOffset(offset);
			spriteCannon_[i]->SetShakeOffset(offset);
			spriteBullet_[i]->SetShakeOffset(offset);
		}
	}
}

void PlayerUI::DebugWithImGui() {
#ifdef _DEBUG

#endif // _DEBUG
}
