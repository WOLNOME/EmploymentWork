#include "ItemUI.h"
#include <WinApp.h>
#include <TextureManager.h>
#include <SpriteManager.h>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>

void ItemUI::Initialize() {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/player");
	//スプライトの初期化
	textureHandles_[0] = TextureManager::GetInstance()->LoadTexture("ItemUI_ReloadSpeedUp.png");
	textureHandles_[1] = TextureManager::GetInstance()->LoadTexture("ItemUI_MoveSpeedUp.png");
	textureHandles_[2] = TextureManager::GetInstance()->LoadTexture("ItemUI_TurnSpeedUp.png");
	for (int i = 0; i < sprites_.size(); i++) {
		sprites_[i] = std::make_unique<Sprite>();
		sprites_[i]->Initialize(SpriteTag{},SpriteManager::GetInstance()->GenerateName("PlayerItem"), Order::Front2, textureHandles_[i]);
		sprites_[i]->SetPosition({ 150.0f,560.0f + (float)i * 60.0f });
		sprites_[i]->SetSize({ (float)kTextureWidth_ / kSpriteStateNum_, (float)kTextureHeight_ });
		sprites_[i]->SetAnchorPoint({ 0.5f, 0.5f });
	}
}

void ItemUI::Update() {
	//プレイヤーがセットされていなければ警告
	if (!player_) {
		assert(0 && "プレイヤーがセットされていません");
	}

	//アイテムの獲得数に応じてUVスクロールさせる
	for (int i = 0; i < sprites_.size(); i++) {
		int itemCount = 0;
		switch (i) {
		case 0: //リロード速度アップ
			itemCount = player_->GetItemReloadSpeedUp();
			break;
		case 1: //移動速度アップ
			itemCount = player_->GetItemMoveSpeedUp();
			break;
		case 2: //回転速度アップ
			itemCount = player_->GetItemTurnSpeedUp();
			break;
		default:
			break;
		}

		float uvScrollRateX = (float)itemCount / kSpriteStateNum_;
		sprites_[i]->SetTextureLeftTop({ uvScrollRateX * kTextureWidth_, 0.0f });
		sprites_[i]->SetTextureSize({ (float)kTextureWidth_ / kSpriteStateNum_, (float)kTextureHeight_ });
	}

}

void ItemUI::AttachShake(const Vector2& _shakeOffset) {
	//スプライトにシェイクを適用する
	for (auto& sprite : sprites_) {
		sprite->SetShakeOffset(_shakeOffset);
	}
}

void ItemUI::AttachBlinking(const Vector4& _color) {
	//スプライトに点滅を適用する
	for (auto& sprite : sprites_) {
		sprite->SetColor(_color);
	}
}
