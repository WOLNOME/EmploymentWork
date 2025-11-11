#include "StartDirection.h"
#include <TextureManager.h>
#include <SpriteManager.h>

void StartDirection::Initialize() {
	input_ = Input::GetInstance();
	isStartDirection_ = true;
	isMaskDisappear_ = false;

	//マスクスプライトの生成・初期化
	uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("blue.png");
	maskSprite_ = std::make_unique<Sprite>();
	maskSprite_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("startMask"), Order::Front0, textureHandle);
	maskSprite_->SetPosition({ 640.0f,360.0f });
	maskSprite_->SetSize({ 1280.0f,720.0f });
	maskSprite_->SetAnchorPoint({ 0.5f,0.5f });
	maskSprite_->SetColor({ 1.0f,1.0f,1.0f,0.5f });
}

void StartDirection::Update() {
	if (isFirstFrame_) {
		isFirstFrame_ = false;

		//メッセージUI
		messageId_ = messageUI_->AddMessage(L"TABでスタート", 999.0f, true);
	}


	//TABキーでスタート演出終了
	if (Input::GetInstance()->TriggerKey(DIK_TAB)) {
		isStartDirection_ = false;
		isMaskDisappear_ = true;

		messageUI_->FinishMessage(messageId_);
	}

	//マスクの更新
	if (isMaskDisappear_) {
		Vector4 color = maskSprite_->GetColor();
		color.w -= 0.02f;
		if (color.w <= 0.0f) {
			color.w = 0.0f;
			isMaskDisappear_ = false;
		}
		maskSprite_->SetColor(color);
	}
}