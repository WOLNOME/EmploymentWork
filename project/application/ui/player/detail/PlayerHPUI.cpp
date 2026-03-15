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

	//バーの初期化
	{
		Vector2 leftTopPos = { param_["hpUI"]["leftTopPos"]["x"],param_["hpUI"]["leftTopPos"]["y"] };
		Vector2 size = { param_["hpUI"]["barSize"]["x"],param_["hpUI"]["barSize"]["y"] };
		thBar_ = TextureManager::GetInstance()->LoadTexture("skyBlue.png");
		spriteBar_ = std::make_unique<Sprite>();
		spriteBar_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("HPBarUI"), Order::Front3, thBar_);
		spriteBar_->SetPosition(leftTopPos);
		spriteBar_->SetSize(size);
	}
	//HPテキストの初期化
	{
		Vector2 textLeftTopPos = { param_["hpUI"]["textLeftTopPos"]["x"],param_["hpUI"]["textLeftTopPos"]["y"] };
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
		spriteHPText_->SetPosition(textLeftTopPos);
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
		Vector2 barSize = { param_["hpUI"]["barSize"]["x"],param_["hpUI"]["barSize"]["y"] };
		barSize.x *= hpRate;
		spriteBar_->SetSize(barSize);
	}
}

void PlayerHPUI::AttachShake(const Vector2& _shakeOffset) {
	//スプライトにシェイクを適用する
	spriteBar_->SetShakeOffset(_shakeOffset);
	spriteHPText_->SetShakeOffset(_shakeOffset);
}

void PlayerHPUI::AttachBlinking(const Vector4& _color) {
	//スプライトに点滅を適用する
	spriteBar_->SetColor(_color);
	spriteHPText_->SetColor(_color);
}