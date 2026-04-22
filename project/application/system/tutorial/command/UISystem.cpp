#include "UISystem.h"
#include <TextureManager.h>
#include <SpriteManager.h>
#include <Input.h>

using namespace Norm;

void UISystem::Initialize() {

	// 初期テクスチャ
	uint32_t textureHandle =
		TextureManager::GetInstance()->LoadTexture("black.png");

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(
		SpriteTag{},
		SpriteManager::GetInstance()->GenerateName("uiSprite"),
		Order::Front5,
		textureHandle
	);

	sprite_->SetAnchorPoint({ 0.5f, 0.5f });
	sprite_->SetPosition({ 640.0f, 360.0f });
	sprite_->SetIsDisplay(false);
}

void UISystem::Update() {
	//表示スプライト
	{
		//表示中なら
		if (sprite_->GetIsDisplay()) {
			//Aボタンを押したら
			if (Input::GetInstance()->TriggerKey(DIK_SPACE)||Input::GetInstance()->TriggerPadButton(GamePadButton::A)) {
				//終了フラグを立てる
				isFinishedShowSprite = true;
				//非表示にする
				sprite_->SetIsDisplay(false);
			}
		}
	}
}

void UISystem::ShowSprite(uint32_t textureHandle) {

	if (!sprite_) {
		return;
	}

	sprite_->SetTexture(textureHandle);
	sprite_->SetIsDisplay(true);
}
