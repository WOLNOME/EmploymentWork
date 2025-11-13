#include "DecorativeUI.h"
#include <WinApp.h>
#include <TextureManager.h>
#include <SpriteManager.h>
#include <cassert>
#include <PostEffectManager.h>

DecorativeUI::~DecorativeUI() {
	//ポストエフェクトを基に戻す
	PostEffectManager::GetInstance()->SetPostEffect(PostEffectKind::None);
}

void DecorativeUI::Initialize() {
	//スプライトの生成・初期化
	textureHandleN_ = TextureManager::GetInstance()->LoadTexture("FPSUI.png");
	textureHandleD_ = TextureManager::GetInstance()->LoadTexture("FPSUI_damage.png");
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("PlayerFPSUI"), Order::Front1, textureHandleN_);
	sprite_->SetAnchorPoint({ 0.5f,0.5f });
	sprite_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });

	//UVスクロールの生成・初期化
	textureHandleND_ = TextureManager::GetInstance()->LoadTexture("FPSUI_damageUS.png");
	uvScroll_ = std::make_unique<Sprite>();
	uvScroll_->Initialize(UVScrollTag{}, SpriteManager::GetInstance()->GenerateName("PlayerFPSUI_UV"), Order::Front1, 8, 0.05f, false, textureHandleND_);
	uvScroll_->SetAnchorPoint({ 0.5f,0.5f });
	uvScroll_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
	uvScroll_->SetIsDisplay(false);
}

void DecorativeUI::Update() {
	//プレイヤーインスタンスのチェック
	assert(player_ != nullptr && "プレイヤーインスタンスがセットされていません");

	//もし死亡演出が始まったら
	if (player_->GetIsDeathDir()) {
		//通常のスプライトを非表示に
		sprite_->SetIsDisplay(false);
		//UVスクロールを表示
		uvScroll_->SetIsDisplay(true);
		//UVスクロールを開始
		uvScroll_->SetIsPlayUVScroll(true);
		//ポストエフェクト
		PostEffectManager::GetInstance()->SetPostEffect(PostEffectKind::Vignette);
	}

	//UVスクロールが完了したら
	if (uvScroll_->GetIsFinishedUVScroll()) {
		//UVスクロールを非表示に
		uvScroll_->SetIsDisplay(false);
		//通常スプライトを表示
		sprite_->SetIsDisplay(true);
		//通常スプライトのテクスチャを差し替え
		sprite_->SetTexture(textureHandleD_);
	}

}

void DecorativeUI::AttachShake(const Vector2& _shakeOffset) {
	//スプライトにシェイクを適用する
	sprite_->SetShakeOffset(_shakeOffset);
	uvScroll_->SetShakeOffset(_shakeOffset);
}

void DecorativeUI::AttachBlinking(const Vector4& _color) {
	//スプライトに点滅を適用する
	sprite_->SetColor(_color);
	uvScroll_->SetColor(_color);
}