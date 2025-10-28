#include "DecorativeUI.h"
#include <WinApp.h>
#include <TextureManager.h>
#include <SpriteManager.h>

void DecorativeUI::Initialize() {
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("FPSUI.png");
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("PlayerFPSUI"), Order::Front1, textureHandle_);
	sprite_->SetAnchorPoint({ 0.5f,0.5f });
	sprite_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
}

void DecorativeUI::AttachShake(const Vector2& _shakeOffset) {
	sprite_->SetShakeOffset(_shakeOffset);
}

void DecorativeUI::AttachBlinking(const Vector4& _color) {
	sprite_->SetColor(_color);
}