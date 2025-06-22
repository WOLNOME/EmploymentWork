#include "PlayerUI.h"
#include "WinApp.h"
#include "TextureManager.h"
#include <random>
#include <cassert>

//アプリケーション
#include "application/object/player/Player.h"

void PlayerUI::Initialize() {
	//パラメータ読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerUI");

	//メンバ変数の生成と初期化
	{
		//2dレティクルの初期化
		th2dReticle_ = TextureManager::GetInstance()->LoadTexture("reticle.png");
		sprite2dReticle_ = std::make_unique<Sprite>();
		sprite2dReticle_->Initialize();
		sprite2dReticle_->SetAnchorPoint({ 0.5f,0.5f });
		sprite2dReticle_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
		sprite2dReticle_->AdjustTextureSize(th2dReticle_);
	}
	{
		//FPSUIの初期化
		thFPSUI_ = TextureManager::GetInstance()->LoadTexture("FPSUI.png");
		spriteFPSUI_ = std::make_unique<Sprite>();
		spriteFPSUI_->Initialize();
		spriteFPSUI_->SetAnchorPoint({ 0.5f,0.5f });
		spriteFPSUI_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
		spriteFPSUI_->AdjustTextureSize(thFPSUI_);
	}
	{
		//HPバーの初期化
		thHPBar_[0] = TextureManager::GetInstance()->LoadTexture("hp_redBar.png");
		thHPBar_[1] = TextureManager::GetInstance()->LoadTexture("hp_greenBar.png");
		for (int i = 0; i < thHPBar_.size(); i++) {
			spriteHPBar_[i] = std::make_unique<Sprite>();
			spriteHPBar_[i]->Initialize();
			spriteHPBar_[i]->SetPosition({ 320.0f,20.0f });
			spriteHPBar_[i]->AdjustTextureSize(thHPBar_[i]);
		}
	}
	{
		//砲弾UIの初期化
		thCannon_[0] = TextureManager::GetInstance()->LoadTexture("cannonUI.png");
		thCannon_[1] = TextureManager::GetInstance()->LoadTexture("black.png");
		for (int i = 0; i < thCannon_.size(); i++) {
			spriteCannon_[i] = std::make_unique<Sprite>();
			spriteCannon_[i]->Initialize();
			spriteCannon_[i]->SetPosition({ 330.0f,565.0f });
			spriteCannon_[i]->AdjustTextureSize(thCannon_[i]);
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
			spriteBullet_[i]->Initialize();
			spriteBullet_[i]->SetPosition({ 830.0f,565.0f });
			spriteBullet_[i]->AdjustTextureSize(thBullet_[i]);
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

}

void PlayerUI::DrawBackSprite() {
}

void PlayerUI::DrawFrontSprite() {
	//スプライトの描画
	sprite2dReticle_->Draw(th2dReticle_);
	//もしカメラが揺れてたらUIも一部揺らす(オフセットはそろえる)
	if (camera_->GetIsShake()) {
		std::random_device seed_gen;
		std::mt19937 engine(seed_gen());
		int shakePower = param_["shakePower"];
		std::uniform_int_distribution<int> dist(-shakePower * camera_->GetShakePower(), shakePower * camera_->GetShakePower());
		//オフセット
		Vector2 offset = { (float)dist(engine),(float)dist(engine) };

		//スプライトのポジションをオフセット分ずらす
		spriteFPSUI_->SetPosition(spriteFPSUI_->GetPosition() + offset);
		for (int i = 0; i < 2; i++) {
			spriteHPBar_[i]->SetPosition(spriteHPBar_[i]->GetPosition() + offset);
			spriteCannon_[i]->SetPosition(spriteCannon_[i]->GetPosition() + offset);
			spriteBullet_[i]->SetPosition(spriteBullet_[i]->GetPosition() + offset);
		}


		//スプライトを描画する
		spriteFPSUI_->Draw(thFPSUI_);
		for (int i = 0; i < 2; i++) {
			spriteHPBar_[i]->Draw(thHPBar_[i]);
			spriteCannon_[i]->Draw(thCannon_[i]);
			spriteBullet_[i]->Draw(thBullet_[i]);
		}

		//描画が終わったのでずらした分元に戻す
		spriteFPSUI_->SetPosition(spriteFPSUI_->GetPosition() - offset);
		for (int i = 0; i < 2; i++) {
			spriteHPBar_[i]->SetPosition(spriteHPBar_[i]->GetPosition() - offset);
			spriteCannon_[i]->SetPosition(spriteCannon_[i]->GetPosition() - offset);
			spriteBullet_[i]->SetPosition(spriteBullet_[i]->GetPosition() - offset);
		}
	}
	//揺れていないので普通に描画する
	else {
		spriteFPSUI_->Draw(thFPSUI_);
		for (int i = 0; i < 2; i++) {
			spriteHPBar_[i]->Draw(thHPBar_[i]);
			spriteCannon_[i]->Draw(thCannon_[i]);
			spriteBullet_[i]->Draw(thBullet_[i]);
		}
	}
}

void PlayerUI::DebugWithImGui() {
#ifdef _DEBUG
	
#endif // _DEBUG
}
