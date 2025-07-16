#include "BulletUI.h"
#include <TextureManager.h>
#include <SpriteManager.h>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>

void BulletUI::Initialize() {
	textureHandles_[0] = TextureManager::GetInstance()->LoadTexture("bulletUI.png");
	textureHandles_[1] = TextureManager::GetInstance()->LoadTexture("black.png");
	for (int i = 0; i < textureHandles_.size(); i++) {
		sprites_[i] = std::make_unique<Sprite>();
		if (i == 0)
			sprites_[i]->Initialize(SpriteManager::GetInstance()->GenerateName("PlayerBulletUI"), Sprite::Order::Front1, textureHandles_[i]);
		else
			sprites_[i]->Initialize(SpriteManager::GetInstance()->GenerateName("PlayerBulletUI"), Sprite::Order::Front2, textureHandles_[i]);
		sprites_[i]->SetPosition({ 830.0f,565.0f });
	}
	sprites_[1]->SetSize({ sprites_[0]->GetSize() });
}

void BulletUI::Update() {
	//プレイヤーがセットされていなければ警告
	assert(player_ != nullptr && "プレイヤーがセットされていません。");

	//銃弾UIのマスクの処理
	if (player_->GetBulletReloadTimer() > 0.0f) {
		//銃弾リロードタイムと同期させる
		float bulletReloadRate = player_->GetBulletReloadTimer() / player_->GetBulletReloadTime();
		sprites_[1]->SetSize({ sprites_[0]->GetSize().x, sprites_[0]->GetSize().y * bulletReloadRate });
	}
	else {
		//残弾数に合わせる
		float bulletNumRate = (float)player_->GetBulletNum() / (float)player_->GetBulletMaxNum();
		sprites_[1]->SetSize({ sprites_[0]->GetSize().x, sprites_[0]->GetSize().y * (1.0f - bulletNumRate) });
	}

}

void BulletUI::AttachShake(const Vector2& _shakeOffset) {
	for (int i = 0; i < 2; i++) {
		sprites_[i]->SetShakeOffset(_shakeOffset);
	}
}

void BulletUI::AttachBlinking(const Vector4& _color) {
	for (int i = 0; i < 2; i++) {
		sprites_[i]->SetColor(_color);
	}
}