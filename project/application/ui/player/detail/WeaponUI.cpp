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
		Vector2 centerPos = { param_["cannonUI"]["centerPos"]["x"],param_["cannonUI"]["centerPos"]["y"] };
		Vector2 reloadUICenterPos = centerPos;
		reloadUICenterPos.x += param_["cannonUI"]["reloadUIOffset"]["x"];
		reloadUICenterPos.y += param_["cannonUI"]["reloadUIOffset"]["y"];
		//基本UI
		thCannon_ = TextureManager::GetInstance()->LoadTexture("cannonUI.png");
		spriteCannon_ = std::make_unique<Sprite>();
		spriteCannon_->Initialize(UVScrollTag{}, SpriteManager::GetInstance()->GenerateName("PlayerCannonUI"), Order::Front1, 2, 0.01f, false, thCannon_);
		spriteCannon_->SetAnchorPoint({ 0.5f,0.5f });
		spriteCannon_->SetPosition(centerPos);
		//リロードUI
		thCannonReload_ = TextureManager::GetInstance()->LoadTexture("reloadUI.png");
		spriteCannonReload_ = std::make_unique<Sprite>();
		spriteCannonReload_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("PlayerCannonReloadUI"), Order::Front1, thCannonReload_);
		spriteCannonReload_->SetAnchorPoint({ 0.5f,0.5f });
		spriteCannonReload_->SetPosition(reloadUICenterPos);
		spriteCannonReload_->SetIsDisplay(false);

	}
	//銃弾
	{
		Vector2 centerPos = { param_["bulletUI"]["centerPos"]["x"],param_["bulletUI"]["centerPos"]["y"] };
		Vector2 magazineUICenterPos = centerPos;
		magazineUICenterPos.x += param_["bulletUI"]["magazineUIOffset"]["x"];
		magazineUICenterPos.y += param_["bulletUI"]["magazineUIOffset"]["y"];
		Vector2 reloadUICenterPos = centerPos;
		reloadUICenterPos.x += param_["bulletUI"]["reloadUIOffset"]["x"];
		reloadUICenterPos.y += param_["bulletUI"]["reloadUIOffset"]["y"];
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
		spriteBullet_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("PlayerBulletUI"), Order::Front1, thBullet_);
		spriteBullet_->SetAnchorPoint({ 0.5f,0.5f });
		spriteBullet_->SetPosition(centerPos);
		//弾数UI
		bulletNumHandle_ = TextTextureManager::GetInstance()->LoadTextTexture(bulletNumTextParam);
		spriteBulletNum_ = std::make_unique<Sprite>();
		spriteBulletNum_->Initialize(TextTag{}, SpriteManager::GetInstance()->GenerateName("PlayerBulletNumUI"), Order::Front1);
		spriteBulletNum_->SetAnchorPoint({ 0.5f,0.5f });
		spriteBulletNum_->SetPosition(magazineUICenterPos);
		spriteBulletNum_->SetTexture(bulletNumHandle_);
		//リロードUI
		thBulletReload_ = TextureManager::GetInstance()->LoadTexture("reloadUI.png");
		spriteBulletReload_ = std::make_unique<Sprite>();
		spriteBulletReload_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("PlayerBulletReloadUI"), Order::Front1, thBulletReload_);
		spriteBulletReload_->SetAnchorPoint({ 0.5f,0.5f });
		spriteBulletReload_->SetPosition(reloadUICenterPos);
		spriteBulletReload_->SetIsDisplay(false);
	}
	//スペシャル
	{
		Vector2 specialUICenterPos = { param_["specialUI"]["centerPos"]["x"],param_["specialUI"]["centerPos"]["y"] };
		thSpecial_ = TextureManager::GetInstance()->LoadTexture("specialUI.png");
		spriteSpecial_ = std::make_unique<Sprite>();
		spriteSpecial_->Initialize(UVScrollTag{}, SpriteManager::GetInstance()->GenerateName("PlayerSpecialUI"), Order::Front1, 4, 0.01f, false, thSpecial_);
		spriteSpecial_->SetAnchorPoint({ 0.5f,0.5f });
		spriteSpecial_->SetPosition(specialUICenterPos);
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
			//α値をリロードの進行度に合わせて変化させる
			float alpha = MyMath::Lerp(0.0f, 1.0f, player_->GetCannonReloadTimer() / playerParam_["cannonReloadTime"].get<float>());
			Vector4 color = spriteCannonReload_->GetColor();
			color.w = alpha;
			spriteCannonReload_->SetColor(color);

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
			//α値をリロードの進行度に合わせて変化させる
			float alpha = MyMath::Lerp(0.0f, 1.0f, player_->GetBulletReloadTimer() / playerParam_["bulletReloadTime"].get<float>());
			Vector4 color = spriteBulletReload_->GetColor();
			color.w = alpha;
			spriteBulletReload_->SetColor(color);
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