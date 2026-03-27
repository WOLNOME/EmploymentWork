#include "ReticleUI.h"
#include <WinApp.h>
#include <TextureManager.h>
#include <SpriteManager.h>

using namespace Norm;

void ReticleUI::Initialize() {
	//パラメーターの取得
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerUI");

	//スプライトの初期化
	{
		Vector2 centerPos = { param_["reticle"]["centerPos"]["x"],param_["reticle"]["centerPos"]["y"] };

		textureHandle_ = TextureManager::GetInstance()->LoadTexture("reticle.png");
		sprite_ = std::make_unique<Sprite>();
		sprite_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("Player2dReticle"), Order::Front1, textureHandle_);
		sprite_->SetAnchorPoint({ 0.5f,0.5f });
		sprite_->SetPosition(centerPos);
	}
}

void ReticleUI::AttachShake(const Vector2& _shakeOffset) {
	sprite_->SetShakeOffset(_shakeOffset);
}

void ReticleUI::AttachBlinking(const Vector4& _color) {
	sprite_->SetColor(_color);

}

void ReticleUI::SetIsDisplay(bool _isDisplay) {
	sprite_->SetIsDisplay(_isDisplay);
}