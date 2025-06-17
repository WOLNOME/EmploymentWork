#include "PlayerUI.h"
#include "WinApp.h"
#include "TextureManager.h"
#include <random>

void PlayerUI::Initialize() {
	//メンバ変数の生成と初期化
	th2dReticle_ = TextureManager::GetInstance()->LoadTexture("reticle.png");
	sprite2dReticle_ = std::make_unique<Sprite>();
	sprite2dReticle_->Initialize();
	sprite2dReticle_->SetAnchorPoint({ 0.5f,0.5f });
	sprite2dReticle_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
	sprite2dReticle_->AdjustTextureSize(th2dReticle_);

	thFPSUI_ = TextureManager::GetInstance()->LoadTexture("FPSUI.png");
	spriteFPSUI_ = std::make_unique<Sprite>();
	spriteFPSUI_->Initialize();
	spriteFPSUI_->SetAnchorPoint({ 0.5f,0.5f });
	spriteFPSUI_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
	spriteFPSUI_->AdjustTextureSize(thFPSUI_);
	
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
		std::uniform_int_distribution<int> dist(-shakePower_, shakePower_);
		//オフセット
		Vector2 offset = { (float)dist(engine),(float)dist(engine) };

		//スプライトのポジションをオフセット分ずらす
		spriteFPSUI_->SetPosition(spriteFPSUI_->GetPosition() + offset);

		//スプライトを描画する
		spriteFPSUI_->Draw(thFPSUI_);

		//描画が終わったのでずらした分元に戻す
		spriteFPSUI_->SetPosition(spriteFPSUI_->GetPosition() - offset);
	}
	//揺れていないので普通に描画する
	else {
		spriteFPSUI_->Draw(thFPSUI_);
	}

}
