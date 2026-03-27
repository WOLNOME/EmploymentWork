#include "DecorativeUI.h"
#include <TextureManager.h>
#include <SpriteManager.h>
#include <cassert>

using namespace Norm;

DecorativeUI::~DecorativeUI() {
	
}

void DecorativeUI::Initialize() {
	//パラメーターの取得
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerUI");

	//スプライトの初期化
	{
		Vector2 centerPos = { param_["decorativeUI"]["centerPos"]["x"],param_["decorativeUI"]["centerPos"]["y"] };
		textureHandle_ = TextureManager::GetInstance()->LoadTexture("decorativeUI.png");
		sprite_ = std::make_unique<Sprite>();
		sprite_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("decorativeUI"), Order::Front1, textureHandle_);
		sprite_->SetAnchorPoint({ 0.5f,0.5f });
		sprite_->SetPosition(centerPos);
	}


	
}

void DecorativeUI::Update() {
	//プレイヤーインスタンスのチェック
	assert(player_ != nullptr && "プレイヤーインスタンスがセットされていません");
}

void DecorativeUI::AttachShake(const Vector2& _shakeOffset) {
	sprite_->SetShakeOffset(_shakeOffset);
}

void DecorativeUI::AttachBlinking(const Vector4& _color) {
	sprite_->SetColor(_color);
	
}

void DecorativeUI::SetIsDisplay(bool _isDisplay) {
	sprite_->SetIsDisplay(_isDisplay);
}