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

	//HPバーの初期化
	thHPBar_[0] = TextureManager::GetInstance()->LoadTexture("hp_redBar.png");
	thHPBar_[1] = TextureManager::GetInstance()->LoadTexture("hp_greenBar.png");
	for (int i = 0; i < thHPBar_.size(); i++) {
		spriteHPBar_[i] = std::make_unique<Sprite>();
		spriteHPBar_[i]->Initialize();
		spriteHPBar_[i]->SetPosition({ 320.0f,20.0f });
		spriteHPBar_[i]->AdjustTextureSize(thHPBar_[i]);
	}
	hpBarWidth_ = spriteHPBar_[0]->GetSize().x;

}

void PlayerUI::Update() {
	//playerが読み込まれていなかったらassert
	assert(player_ != nullptr && "PlayerUIにPlayerインスタンスを渡してください");

	//HPバーのサイズをプレイヤーのHPに合わせる
	float hpRate = (float)player_->GetHP() / (float)player_->GetMaxHP();
	spriteHPBar_[1]->SetSize({ hpBarWidth_ * hpRate,spriteHPBar_[1]->GetSize().y });

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
		std::uniform_int_distribution<int> dist(-shakePower*camera_->GetShakePower(), shakePower * camera_->GetShakePower());
		//オフセット
		Vector2 offset = { (float)dist(engine),(float)dist(engine) };

		//スプライトのポジションをオフセット分ずらす
		spriteFPSUI_->SetPosition(spriteFPSUI_->GetPosition() + offset);
		for (int i = 0; i < 2; i++) {
			spriteHPBar_[i]->SetPosition(spriteHPBar_[i]->GetPosition() + offset);
		}

		//スプライトを描画する
		spriteFPSUI_->Draw(thFPSUI_);
		for (int i = 0; i < 2; i++) {
			spriteHPBar_[i]->Draw(thHPBar_[i]);
		}

		//描画が終わったのでずらした分元に戻す
		spriteFPSUI_->SetPosition(spriteFPSUI_->GetPosition() - offset);
		for (int i = 0; i < 2; i++) {
			spriteHPBar_[i]->SetPosition(spriteHPBar_[i]->GetPosition() - offset);
		}
	}
	//揺れていないので普通に描画する
	else {
		spriteFPSUI_->Draw(thFPSUI_);
		for (int i = 0; i < 2; i++) {
			spriteHPBar_[i]->Draw(thHPBar_[i]);
		}
	}
}

void PlayerUI::DebugWithImGui() {
#ifdef _DEBUG
	for (int i = 0; i < 2; i++) {
		spriteHPBar_[i]->DebugWithImGui();
	}
#endif // _DEBUG
}
