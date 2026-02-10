#include "BulletUI.h"
#include <TextureManager.h>
#include <SpriteManager.h>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>

using namespace Norm;

void BulletUI::Initialize() {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/player");
	//スプライトの初期化
	textureHandles_[0] = TextureManager::GetInstance()->LoadTexture("bulletUI.png");
	textureHandles_[1] = TextureManager::GetInstance()->LoadTexture("black.png");
	for (int i = 0; i < textureHandles_.size(); i++) {
		sprites_[i] = std::make_unique<Sprite>();
		if (i == 0)
			sprites_[i]->Initialize(SpriteTag{},SpriteManager::GetInstance()->GenerateName("PlayerBulletUI"), Order::Front2, textureHandles_[i]);
		else
			sprites_[i]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("PlayerBulletUI"), Order::Front3, textureHandles_[i]);
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
		float reloadTime = param_["bulletReloadTime"];
		float bulletReloadRate = player_->GetBulletReloadTimer() / reloadTime;
		sprites_[1]->SetSize({ sprites_[0]->GetSize().x, sprites_[0]->GetSize().y * bulletReloadRate });
	}
	else {
		//残弾数に合わせる
		int bulletMaxNum = param_["bulletMagazine"];
		float bulletNumRate = (float)player_->GetBulletNum() / (float)bulletMaxNum;
		sprites_[1]->SetSize({ sprites_[0]->GetSize().x, sprites_[0]->GetSize().y * (1.0f - bulletNumRate) });
	}

}

void BulletUI::AttachShake(const Vector2& _shakeOffset) {
	//スプライトにシェイクを適用する
	for (int i = 0; i < 2; i++) {
		sprites_[i]->SetShakeOffset(_shakeOffset);
	}
}

void BulletUI::AttachBlinking(const Vector4& _color) {
	//スプライトに点滅を適用する
	for (int i = 0; i < 2; i++) {
		sprites_[i]->SetColor(_color);
	}
}