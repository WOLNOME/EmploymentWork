#include "MessageSystem.h"
#include <algorithm>
#include <TextureManager.h>
#include <TextTextureManager.h>
#include <SpriteManager.h>
#include <Input.h>

using namespace Norm;

void MessageSystem::Initialize() {

	// ウィンドウ
	{
		uint32_t textureHandle =
			TextureManager::GetInstance()->LoadTexture("tutorial_messageWindow.png");

		windowSprite_ = std::make_unique<Sprite>();
		windowSprite_->Initialize(
			SpriteTag{},
			SpriteManager::GetInstance()->GenerateName("messageWindow"),
			Order::Front4,
			textureHandle
		);

		windowSprite_->SetAnchorPoint({ 0.5f,0.5f });
		windowSprite_->SetPosition(centerPos_);
		windowSprite_->SetIsDisplay(false);
	}

	// 次へUI
	{
		uint32_t textureHandle =
			TextureManager::GetInstance()->LoadTexture("A.png");

		nextUISprite_ = std::make_unique<Sprite>();
		nextUISprite_->Initialize(
			SpriteTag{},
			SpriteManager::GetInstance()->GenerateName("nextUI"),
			Order::Front5,
			textureHandle
		);

		nextUISprite_->SetAnchorPoint({ 0.5f,0.5f });
		nextUISprite_->SetPosition({ centerPos_.x + 480.0f, centerPos_.y + 62.0f });
		nextUISprite_->SetIsDisplay(false);
	}

	// テキスト
	{
		TextParam param;
		param.text = L"";
		param.font = Font::UDDegitalNP_B;
		param.fontStyle = FontStyle::Normal;
		param.size = 32.0f;
		param.color = { 1,1,1,1 };

		textHandle_ =
			TextTextureManager::GetInstance()->LoadTextTexture(param);

		textSprite_ = std::make_unique<Sprite>();
		textSprite_->Initialize(
			TextTag{},
			SpriteManager::GetInstance()->GenerateName("text"),
			Order::Front5
		);

		textSprite_->SetTexture(textHandle_);
		textSprite_->SetAnchorPoint({ 0.5f,0.5f });
		textSprite_->SetPosition(centerPos_);
		textSprite_->SetIsDisplay(false);
	}
}

void MessageSystem::Update() {
	//テキストウィンドウの更新
	UpdateWindow();
	//テキストの更新
	UpdateText();

}

bool MessageSystem::OpenWindow() {
	if (isOpenWindow_) {
		return false;
	}

	isOpenWindow_ = true;
	isDirectionWindow_ = true;
	isWindowOpened_ = false;

	return true;
}

bool MessageSystem::CloseWindow() {
	if (!isOpenWindow_) {
		return false;
	}

	isOpenWindow_ = false;
	isDirectionWindow_ = true;
	isWindowClosed_ = false;

	return true;
}

void MessageSystem::ShowText(const std::wstring& text, bool isAttachNextUI) {
	allMessage_ = text;
	currentMessage_.clear();

	TextTextureManager::GetInstance()->EditTextString(
		textHandle_, currentMessage_);

	textSprite_->SetIsDisplay(true);
	nextUISprite_->SetIsDisplay(isAttachNextUI);

	inputTimer_ = 0.0f;
	isAttachNextUI_ = isAttachNextUI;
	blinkingTimer_ = 0.0f;
}

void MessageSystem::ClearText() {

	allMessage_.clear();
	currentMessage_.clear();
	textSprite_->SetIsDisplay(false);
}

void MessageSystem::UpdateWindow() {

	// ウィンドウ演出中でなければ何もしない
	if (!isDirectionWindow_) {
		return;
	}

	if (isOpenWindow_) {
		dirTimer_ += kDeltaTime;
	}
	else {
		dirTimer_ -= kDeltaTime;
	}

	dirTimer_ = std::clamp(dirTimer_, 0.0f, dirDuration_);

	float alpha = dirTimer_ / dirDuration_;

	// ウィンドウの透明度更新
	windowSprite_->SetColor({ 1,1,1,alpha });

	// フェードアウト完了
	if (!isOpenWindow_ && dirTimer_ <= 0.0f) {
		windowSprite_->SetIsDisplay(false);
		nextUISprite_->SetIsDisplay(false);

		isDirectionWindow_ = false;
		isWindowClosed_ = true;

		return;
	}

	// フェードイン完了
	if (isOpenWindow_ && dirTimer_ >= dirDuration_) {
		isDirectionWindow_ = false;
		isWindowOpened_ = true;
	}

	windowSprite_->SetIsDisplay(true);
}

void MessageSystem::UpdateText() {
	Input* input = Input::GetInstance();

	if (!textSprite_->GetIsDisplay()) {
		return;
	}

	// タイプライター表示
	if (currentMessage_.size() < allMessage_.size()) {

		inputTimer_ += kDeltaTime;

		if (inputTimer_ >= inputDuration_) {

			inputTimer_ = 0.0f;

			currentMessage_ +=
				allMessage_[currentMessage_.size()];

			TextTextureManager::GetInstance()->EditTextString(
				textHandle_,currentMessage_);
		}
	}
	else if (isAttachNextUI_) {
		blinkingTimer_ += kDeltaTime;

		if (blinkingTimer_ >= blinkingDuration_) {
			blinkingTimer_ = 0.0f;
		}

		float blinkAlpha =
			std::sin((blinkingTimer_ / blinkingDuration_) * 3.1415f);

		nextUISprite_->SetColor({ 1,1,1,blinkAlpha });

		if (input->TriggerKey(DIK_SPACE)||input->TriggerPadButton(GamePadButton::A)) {
			isNextAdvance_ = true;
			ClearText();
		}
	}
}
