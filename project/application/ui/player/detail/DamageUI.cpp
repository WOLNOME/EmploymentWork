#include "DamageUI.h"
#include <TextureManager.h>
#include <SpriteManager.h>
#include <cassert>
#include <PostEffectManager.h>

using namespace Norm;

DamageUI::~DamageUI() {
	//ポストエフェクトを基に戻す
	PostEffectManager::GetInstance()->SetPostEffect(PostEffectKind::None);
}

void DamageUI::Initialize() {
	//パラメーターの取得
	param_= JsonUtil::GetJsonData("Resources/parameters/playerUI");
	playerParam_=JsonUtil::GetJsonData("Resources/parameters/player");

	//窓ガラスの割れたスプライトの初期化
	{
		Vector2 centerPos = { param_["damageUI"]["centerPos"]["x"],param_["damageUI"]["centerPos"]["y"] };

		thGlass_ = TextureManager::GetInstance()->LoadTexture("damageUI.png");
		spriteGrass_ = std::make_unique<Sprite>();
		spriteGrass_->Initialize(UVScrollTag{}, SpriteManager::GetInstance()->GenerateName("DamageUI"), Order::Front0,3,0.01f,false,thGlass_);
		spriteGrass_->SetAnchorPoint({ 0.5f,0.5f });
		spriteGrass_->SetPosition(centerPos);

	}
	//死亡時のガラス演出
	{
		Vector2 centerPos = { param_["damageUI"]["centerPos"]["x"],param_["damageUI"]["centerPos"]["y"] };

		thDeathDir_ = TextureManager::GetInstance()->LoadTexture("deathdirectionUI.png");
		spriteDeathDir_ = std::make_unique<Sprite>();
		spriteDeathDir_->Initialize(UVScrollTag{}, SpriteManager::GetInstance()->GenerateName("DeathDirectionUI"), Order::Front0, 8, 0.05f, false, thDeathDir_);
		spriteDeathDir_->SetAnchorPoint({ 0.5f,0.5f });
		spriteDeathDir_->SetPosition(centerPos);
		spriteDeathDir_->SetIsDisplay(false);
	}
}

void DamageUI::Update() {
	//プレイヤーインスタンスのチェック
	assert(player_ != nullptr && "プレイヤーインスタンスがセットされていません");

	//ダメージスプライトの切り替え
	{
		int hp = player_->GetHP();
		//プレイヤーの体力が1/2より大きいなら
		if (hp > playerParam_["maxHP"].get<int>() / 2) {
			spriteGrass_->SetUVScrollSheetNum(0);
		}
		//プレイヤーの体力が1/2~1/4だったら
		else if (hp <= playerParam_["maxHP"].get<int>() / 2 && hp > playerParam_["maxHP"].get<int>() / 4) {
			spriteGrass_->SetUVScrollSheetNum(1);
		}
		//プレイヤーの体力が20以下だったら
		else {
			spriteGrass_->SetUVScrollSheetNum(2);
		}
	}
	//死亡時のガラス演出
	{
		if (player_->GetIsDeathDir()) {
			//通常のスプライトを非表示に
			spriteGrass_->SetIsDisplay(false);
			//UVスクロールを表示
			spriteDeathDir_->SetIsDisplay(true);
			//UVスクロールを開始
			spriteDeathDir_->SetIsPlayUVScroll(true);
			//ポストエフェクト
			PostEffectManager::GetInstance()->SetPostEffect(PostEffectKind::Vignette);
		}
	}

}


void DamageUI::AttachShake(const Vector2& _shakeOffset) {
	spriteGrass_->SetShakeOffset(_shakeOffset);
	spriteDeathDir_->SetShakeOffset(_shakeOffset);

}

void DamageUI::AttachBlinking(const Vector4& _color) {
	spriteGrass_->SetColor(_color);
	spriteDeathDir_->SetColor(_color);

}