#include "WeaponUI.h"
#include <TextureManager.h>
#include <TextTextureManager.h>
#include <SpriteManager.h>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>

using namespace Norm;

void WeaponUI::Initialize() {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerUI");
	playerParam_ = JsonUtil::GetJsonData("Resources/parameters/player");

	//スプライトの生成・初期化

	//砲弾
	{
		Vector2 leftTopPos = { param_["cannonUI"]["leftTopPos"]["x"],param_["cannonUI"]["leftTopPos"]["y"] };
		//基本UI
		thCannon_ = TextureManager::GetInstance()->LoadTexture("cannonUI.png");
		spriteCannon_ = std::make_unique<Sprite>();
		spriteCannon_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("PlayerCannonUI"), Order::Front2, thCannon_);
		spriteCannon_->SetPosition(leftTopPos);
		//リロードUI
		thCannonReload_ = TextureManager::GetInstance()->LoadTexture("reloadUI.png");
		spriteCannonReload_ = std::make_unique<Sprite>();
		spriteCannonReload_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("PlayerCannonReloadUI"), Order::Front3, thCannonReload_);
		spriteCannonReload_->SetPosition(leftTopPos);
		spriteCannonReload_->SetIsDisplay(false);

	}
	//銃弾
	{
		Vector2 leftTopPos = { param_["bulletUI"]["leftTopPos"]["x"],param_["bulletUI"]["leftTopPos"]["y"] };
		Vector2 magazineUILeftTopPos;
		magazineUILeftTopPos.x += param_["bulletUI"]["magazineUILeftTopPos"]["x"];
		magazineUILeftTopPos.y += param_["bulletUI"]["magazineUILeftTopPos"]["y"];
		TextParam bulletNumTextParam = {
			.text = L"",
			.font = Font::UDDegitalN_B,
			.fontStyle = FontStyle::Normal,
			.size = 48.0f,
			.color = Vector4(0.224f,0.769f,0.584f,1.0f),
		};
		//基本UI
		thBullet_ = TextureManager::GetInstance()->LoadTexture("bulletUI.png");
		spriteBullet_ = std::make_unique<Sprite>();
		spriteBullet_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("PlayerBulletUI"), Order::Front2, thBullet_);
		spriteBullet_->SetPosition(leftTopPos);
		//弾数UI
		bulletNumHandle_ = TextTextureManager::GetInstance()->LoadTextTexture(bulletNumTextParam);
		spriteBulletNum_ = std::make_unique<Sprite>();
		spriteBulletNum_->Initialize(TextTag{}, SpriteManager::GetInstance()->GenerateName("PlayerBulletNumUI"), Order::Front2);
		spriteBulletNum_->SetPosition(magazineUILeftTopPos);
		spriteBulletNum_->SetTexture(bulletNumHandle_);
		//リロードUI
		thBulletReload_ = TextureManager::GetInstance()->LoadTexture("reloadUI.png");
		spriteBulletReload_ = std::make_unique<Sprite>();
		spriteBulletReload_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("PlayerBulletReloadUI"), Order::Front3, thBulletReload_);
		spriteBulletReload_->SetPosition(leftTopPos);
		spriteBulletReload_->SetIsDisplay(false);
	}
	//スペシャル
	{
		Vector2 leftTopPos = { param_["specialUI"]["leftTopPos"]["x"],param_["specialUI"]["leftTopPos"]["y"] };
		thSpecial_ = TextureManager::GetInstance()->LoadTexture("specialUI.png");
		spriteSpecial_ = std::make_unique<Sprite>();
		spriteSpecial_->Initialize(UVScrollTag{}, SpriteManager::GetInstance()->GenerateName("PlayerSpecialUI"), Order::Front2, 4, 0.01f, false, thSpecial_);
		spriteSpecial_->SetPosition(leftTopPos);
	}
}

void WeaponUI::Update() {
	//プレイヤーがセットされていなければ警告
	assert(player_ != nullptr && "プレイヤーがセットされていません。");

	//砲弾UIの更新
	{
		//砲弾のリロード中
		if (player_->GetCannonReloadTimer() > 0.0f) {
			spriteCannon_->SetUVScrollSheetNum(1);
			//リロードUIを表示
			spriteCannonReload_->SetIsDisplay(true);
			//UVスクロール
			Vector2 size = {
				spriteCannon_->GetSize().x * (player_->GetCannonReloadTimer() / playerParam_["cannonReloadTime"].get<float>()),
				spriteCannon_->GetSize().y
			};
			spriteCannonReload_->SetSize(size);
			spriteCannonReload_->SetTextureSize(size);

		}
		//装填済み
		else {
			spriteCannon_->SetUVScrollSheetNum(0);
			//リロードUIは非表示
			spriteCannonReload_->SetIsDisplay(false);
		}
	}
	//バレットUIの更新
	{
		//弾数をテクスチャに反映
		int maxBulletNum = playerParam_["bulletMagazine"].get<int>();
		int bulletNum = player_->GetBulletNum();
		TextTextureManager::GetInstance()->EditTextString(bulletNumHandle_, L"{}/{}", bulletNum, maxBulletNum);
		//銃弾のリロード中
		if (player_->GetBulletReloadTimer() > 0.0f) {
			//リロードUIを表示
			spriteBulletReload_->SetIsDisplay(true);

			//UVスクロール
			Vector2 size = {
				spriteBullet_->GetSize().x * (player_->GetBulletReloadTimer() / playerParam_["bulletReloadTime"].get<float>()),
				spriteBullet_->GetSize().y
			};
			spriteBulletReload_->SetSize(size);
			spriteBulletReload_->SetTextureSize(size);
		}
		//装填済み
		else {
			//リロードUIは非表示
			spriteBulletReload_->SetIsDisplay(false);
		}
	}

	//スペシャルUIの更新
	{
		//スペシャルの数に合わせてUVシートを切り替える
		spriteSpecial_->SetUVScrollSheetNum(player_->GetSpecialNum());
	}
}

void WeaponUI::AttachShake(const Norm::Vector2& _shakeOffset) {
	spriteCannon_->SetShakeOffset(_shakeOffset);
	spriteCannonReload_->SetShakeOffset(_shakeOffset);
	spriteBullet_->SetShakeOffset(_shakeOffset);
	spriteBulletNum_->SetShakeOffset(_shakeOffset);
	spriteBulletReload_->SetShakeOffset(_shakeOffset);
	spriteSpecial_->SetShakeOffset(_shakeOffset);
}

void WeaponUI::AttachBlinking(const Norm::Vector4& _color) {
	spriteCannon_->SetColor(_color);
	spriteBullet_->SetColor(_color);
	spriteBulletNum_->SetColor(_color);
	spriteSpecial_->SetColor(_color);
}