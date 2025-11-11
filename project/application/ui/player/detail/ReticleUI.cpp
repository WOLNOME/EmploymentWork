#include "ReticleUI.h"
#include <WinApp.h>
#include <TextureManager.h>
#include <SpriteManager.h>

void ReticleUI::Initialize() {
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("reticle.png");
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("Player2dReticle"), Order::Front1, textureHandle_);
	sprite_->SetAnchorPoint({ 0.5f,0.5f });
	sprite_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
}
