#include "CannonUI.h"
#include <TextureManager.h>
#include <SpriteManager.h>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>

void CannonUI::Initialize() {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/player");
	//スプライトの初期化
	textureHandles_[0] = TextureManager::GetInstance()->LoadTexture("cannonUI.png");
	textureHandles_[1] = TextureManager::GetInstance()->LoadTexture("black.png");
	for (int i = 0; i < textureHandles_.size(); i++) {
		sprites_[i] = std::make_unique<Sprite>();
		if (i == 0)
			sprites_[i]->Initialize(SpriteTag{},SpriteManager::GetInstance()->GenerateName("PlayerCannonUI"), Order::Front2, textureHandles_[i]);
		else
			sprites_[i]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("PlayerCannonUI"), Order::Front3, textureHandles_[i]);
		sprites_[i]->SetPosition({ 330.0f,565.0f });
	}
	sprites_[1]->SetSize({ sprites_[0]->GetSize() });
}

void CannonUI::Update() {
	//プレイヤーがセットされていなければ警告
	assert(player_ != nullptr && "プレイヤーがセットされていません。");

	//砲弾UIのマスクを砲弾クールタイムと同期させる
	float reloadTime = param_["cannonReloadTime"];
	float reloadSpeedUpValue = param_["item_reloadSpeedUpValue"];
	reloadTime -= player_->GetItemReloadSpeedUp() * reloadSpeedUpValue;
	float cannonBallCoolRate = player_->GetCannonReloadTimer() / reloadTime;
	sprites_[1]->SetSize({ sprites_[0]->GetSize().x, sprites_[0]->GetSize().y * cannonBallCoolRate });

}

void CannonUI::AttachShake(const Vector2& _shakeOffset) {
	//スプライトにシェイクを適用する
	for (int i = 0; i < 2; i++) {
		sprites_[i]->SetShakeOffset(_shakeOffset);
	}
}

void CannonUI::AttachBlinking(const Vector4& _color) {
	//スプライトに点滅を適用する
	for (int i = 0; i < 2; i++) {
		sprites_[i]->SetColor(_color);
	}
}