#include "PlayerHPUI.h"
#include <TextTextureManager.h>
#include <TextureManager.h>
#include <SpriteManager.h>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>

using namespace Norm;

void PlayerHPUI::Initialize() {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerUI");
	playerParam_ = JsonUtil::GetJsonData("Resources/parameters/player");

	//基盤の初期化
	{
		Vector2 centerPos = { param_["hpUI"]["centerPos"]["x"],param_["hpUI"]["centerPos"]["y"] };
		thBase_ = TextureManager::GetInstance()->LoadTexture("hpBase.png");
		spriteBase_ = std::make_unique<Sprite>();
		spriteBase_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("hpBaseUI"), Order::Front2, thBase_);
		spriteBase_->SetAnchorPoint({ 0.5f,0.5f });
		spriteBase_->SetPosition(centerPos);
	}
	//バーの初期化
	{
		Vector2 centerPos = { param_["hpUI"]["centerPos"]["x"],param_["hpUI"]["centerPos"]["y"] };
		centerPos.x += param_["hpUI"]["barOffset"]["x"];
		centerPos.y += param_["hpUI"]["barOffset"]["y"];

		thBar_[0] = TextureManager::GetInstance()->LoadTexture("redBar.png");
		thBar_[1] = TextureManager::GetInstance()->LoadTexture("greenBar.png");
		spriteBar_[0] = std::make_unique<Sprite>();
		spriteBar_[1] = std::make_unique<Sprite>();
		spriteBar_[0]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("redBarUI"), Order::Front3, thBar_[0]);
		spriteBar_[1]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("greenBarUI"), Order::Front4, thBar_[1]);
		spriteBar_[0]->SetAnchorPoint({ 0.0f,0.5f });
		spriteBar_[1]->SetAnchorPoint({ 0.0f,0.5f });
		spriteBar_[0]->SetPosition(centerPos);
		spriteBar_[1]->SetPosition(centerPos);
	}
	//HPテキストの初期化
	{
		Vector2 centerPos = { param_["hpUI"]["centerPos"]["x"],param_["hpUI"]["centerPos"]["y"] };
		centerPos.x += param_["hpUI"]["textOffset"]["x"];
		centerPos.y += param_["hpUI"]["textOffset"]["y"];
		TextParam hpTextParam = {
			.text = L"",
			.font = Font::UDDegitalN_B,
			.fontStyle = FontStyle::Normal,
			.size = 32.0f,
			.color = Vector4(0.224f,0.769f,0.584f,1.0f),
		};

		thHPText_ = TextTextureManager::GetInstance()->LoadTextTexture(hpTextParam);
		spriteHPText_ = std::make_unique<Sprite>();
		spriteHPText_->Initialize(TextTag{}, SpriteManager::GetInstance()->GenerateName("hpTextUI"), Order::Front3);
		spriteHPText_->SetAnchorPoint({ 0.5f,0.5f });
		spriteHPText_->SetPosition(centerPos);
		spriteHPText_->SetTexture(thHPText_);
	}

}

void PlayerHPUI::Update() {
	//プレイヤーがセットされていなければ警告
	assert(player_ && "プレイヤーがセットされていません");

	//HPテキストの更新
	{
		int hp = player_->GetHP();
		int maxHp = playerParam_["maxHP"];
		TextTextureManager::GetInstance()->EditTextString(thHPText_,L"{}/{}",hp,maxHp);
	}
	//HPバーの更新
	{
		int hp = player_->GetHP();
		int maxHp = playerParam_["maxHP"];
		float hpRate = (float)hp / maxHp;
		Vector2 barSize = spriteBar_[0]->GetSize();
		barSize.x *= hpRate;
		spriteBar_[1]->SetSize(barSize);
	}


}

void PlayerHPUI::AttachShake(const Vector2& _shakeOffset) {
	//スプライトにシェイクを適用する
	spriteBase_->SetShakeOffset(_shakeOffset);
	spriteBar_[0]->SetShakeOffset(_shakeOffset);
	spriteBar_[1]->SetShakeOffset(_shakeOffset);
	spriteHPText_->SetShakeOffset(_shakeOffset);
}

void PlayerHPUI::AttachBlinking(const Vector4& _color) {
	//スプライトに点滅を適用する
	spriteBase_->SetColor(_color);
	spriteBar_[0]->SetColor(_color);
	spriteBar_[1]->SetColor(_color);
	spriteHPText_->SetColor(_color);
}