#include "KeyUI.h"
#include <TextureManager.h>
#include <SpriteManager.h>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>

using namespace Norm;

void KeyUI::Initialize() {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerUI");
	playerParam_ = JsonUtil::GetJsonData("Resources/parameters/player");

	//スプライトの生成・初期化
	//キー
	{
		Vector2 keyUILeftTopPos = { param_["key"]["leftTopPos"]["x"],param_["key"]["leftTopPos"]["y"]};
		thKey_ = TextureManager::GetInstance()->LoadTexture("keyUI.png");
		spriteKey_ = std::make_unique<Sprite>();
		spriteKey_->Initialize(UVScrollTag{}, SpriteManager::GetInstance()->GenerateName("PlayerKeyUI"), Order::Front2, 3, 0.01f, false, thKey_);
		spriteKey_->SetPosition(keyUILeftTopPos);
	}
}

void KeyUI::Update() {
	//プレイヤーがセットされていなければ警告
	assert(player_ != nullptr && "プレイヤーがセットされていません。");
	//キーUIの更新
	{
		int keyNum = player_->GetKeyNum();
		spriteKey_->SetUVScrollSheetNum(keyNum);
	}
}

void KeyUI::AttachShake(const Norm::Vector2& _shakeOffset) {
	spriteKey_->SetShakeOffset(_shakeOffset);
}

void KeyUI::AttachBlinking(const Norm::Vector4& _color) {
	spriteKey_->SetColor(_color);
}

void KeyUI::SetIsDisplay(bool _isDisplay) {
	spriteKey_->SetIsDisplay(_isDisplay);
}