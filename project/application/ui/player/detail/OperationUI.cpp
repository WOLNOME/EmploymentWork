#include "OperationUI.h"
#include <TextureManager.h>
#include <SpriteManager.h>
#include <ImGuiManager.h>

using namespace Norm;

void OperationUI::Initialize() {
	//パラメータの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerUI");

	//変数の初期化
	centerPos_ = { param_["operationUI"]["centerPos"]["x"],param_["operationUI"]["centerPos"]["y"] };


	//移動UIスプライトの初期化
	textureHandle_ = TextureManager::GetInstance()->LoadTexture("operationUI.png");
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("operationUI"), Order::Front2, textureHandle_);
	sprite_->SetAnchorPoint({ 0.5f,0.5f });
	sprite_->SetPosition(centerPos_);
}

void OperationUI::Update() {

}

void OperationUI::AttachShake(const Vector2& _shakeOffset) {
	_shakeOffset;
}

void OperationUI::AttachBlinking(const Vector4& _color) {
	_color;
}

void OperationUI::SetIsDisplay(bool _isDisplay) {
	sprite_->SetIsDisplay(_isDisplay);
}