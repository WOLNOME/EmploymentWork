#include "PlayerHPUI.h"
#include <TextureManager.h>
#include <SpriteManager.h>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>

void PlayerHPUI::Initialize() {
	param_ = JsonUtil::GetJsonData("Resources/parameters/player");

	textureHandles_[0] = TextureManager::GetInstance()->LoadTexture("hp_redBar.png");
	textureHandles_[1] = TextureManager::GetInstance()->LoadTexture("hp_greenBar.png");
	for (int i = 0; i < sprites_.size(); i++) {
		sprites_[i] = std::make_unique<Sprite>();
		if (i == 0)
			sprites_[i]->Initialize(SpriteManager::GetInstance()->GenerateName("PlayerHPBar"), Sprite::Order::Front2, textureHandles_[i]);
		else
			sprites_[i]->Initialize(SpriteManager::GetInstance()->GenerateName("PlayerHPBar"), Sprite::Order::Front3, textureHandles_[i]);
		sprites_[i]->SetPosition({ 320.0f,20.0f });
	}
}

void PlayerHPUI::Update() {
	//プレイヤーがセットされていなければ警告
	if (!player_) {
		assert(0 && "プレイヤーがセットされていません");
	}

	//緑HPバーのサイズをプレイヤーのHPに合わせる
	int maxHP = param_["maxHP"];
	float hpRate = (float)player_->GetHP() / (float)maxHP;
	sprites_[1]->SetSize({ sprites_[0]->GetSize().x * hpRate,sprites_[0]->GetSize().y });
}

void PlayerHPUI::AttachShake(const Vector2& _shakeOffset) {
	for (int i = 0; i < 2; i++) {
		sprites_[i]->SetShakeOffset(_shakeOffset);
	}
}

void PlayerHPUI::AttachBlinking(const Vector4& _color) {
	for (int i = 0; i < 2; i++) {
		sprites_[i]->SetColor(_color);
	}
}