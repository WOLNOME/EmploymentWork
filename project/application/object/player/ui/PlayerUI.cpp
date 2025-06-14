#include "PlayerUI.h"
#include "WinApp.h"
#include "TextureManager.h"
#include <random>

void PlayerUI::Initialize() {
	//メンバ変数の生成と初期化
	th2dReticle_ = TextureManager::GetInstance()->LoadTexture("reticle.png");
	sprite2dReticle_ = std::make_unique<Sprite>();
	sprite2dReticle_->Initialize(th2dReticle_);
	sprite2dReticle_->SetAnchorPoint({ 0.5f,0.5f });
	sprite2dReticle_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });

	thFPSUI_ = TextureManager::GetInstance()->LoadTexture("FPSUI.png");
	spriteFPSUI_ = std::make_unique<Sprite>();
	spriteFPSUI_->Initialize(thFPSUI_);
	spriteFPSUI_->SetAnchorPoint({ 0.5f,0.5f });
	spriteFPSUI_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
	
}

void PlayerUI::Update() {
	//スプライトの更新
	sprite2dReticle_->Update();
	//もしカメラが揺れてたらUIも一部揺らす(オフセットはそろえる)
	if (camera_->GetIsShake()) {
		std::random_device seed_gen;
		std::mt19937 engine(seed_gen());
		std::uniform_int_distribution<int> dist(- shakePower_, shakePower_);
		//オフセット
		Vector2 offset = {(float)dist(engine),(float)dist(engine)};
		
		//スプライトのポジションをオフセット分ずらす
		spriteFPSUI_->SetPosition(spriteFPSUI_->GetPosition() + offset);

		//スプライトを更新する
		spriteFPSUI_->Update();

		//更新が終わったのでずらした分元に戻す
		spriteFPSUI_->SetPosition(spriteFPSUI_->GetPosition() - offset);
	}
	//揺れていないので普通に更新する
	else {
		spriteFPSUI_->Update();
	}

}

void PlayerUI::DrawBackSprite() {
}

void PlayerUI::DrawFrontSprite() {
	//2dレティクルの描画
	sprite2dReticle_->Draw();
	//FPSUIの描画
	spriteFPSUI_->Draw();
}
