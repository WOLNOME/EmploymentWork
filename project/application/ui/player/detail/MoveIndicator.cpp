#include "MoveIndicator.h"
#include <WinApp.h>
#include <TextureManager.h>
#include <SpriteManager.h>
#include <MyMath.h>
#include <cassert>

//アプリケーション
#include <application/system/CameraManager.h>
#include <application/object/character/player/Player.h>

using namespace Norm;

void MoveIndicator::Initialize() {

	//パラメーターの初期化
	playerParam_ = JsonUtil::GetJsonData("Resources/parameters/player");
	playerUIParam_ = JsonUtil::GetJsonData("Resources/parameters/playerUI");

	//テクスチャ
	textureHadle_ = TextureManager::GetInstance()->LoadTexture("MoveIndicator.png");
	//スプライト
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("moveIndicator"), Order::Front4, textureHadle_);
	sprite_->SetAnchorPoint({ 0.5f,0.5f });
	sprite_->SetPosition({ WinApp::GetInstance()->kClientWidth / 2.0f,WinApp::GetInstance()->kClientHeight / 2.0f });
	sprite_->SetIsDisplay(true);

}

void MoveIndicator::Update() {
	//プレイヤーがセットされていなかったら警告
	assert(player_ && "プレイヤーがセットされていません");
	//カメラマネージャーがセットされていなかったら警告
	assert(cameraManager_ && "カメラマネージャーがセットされていません");

	//使用する変数
	Vector3 p = player_->GetWorldTransform().GetTranslate();	//プレイヤーの座標
	Vector3 v1 = {
		std::sinf(cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y),
		0.0f,
		std::cosf(cameraManager_->GetActiveCamera()->worldTransform.GetRotate().y)
	};	//カメラの向き
	Vector3 v2 = {
		std::sinf(player_->GetWorldTransform().GetRotate().y),
		0.0f,
		std::cosf(player_->GetWorldTransform().GetRotate().y)
	};	//車体の向き

	//外積と内積から角度を求める
	float dot = MyMath::Dot(v1, v2);
	float cross = MyMath::Cross(v1, v2).y;

	//atan2で回転角を得る
	float rotation = std::atan2f(cross, dot);

	//スプライトに適用
	sprite_->SetRotation(rotation);

}
