#include "DirectionUI.h"
#include <TextureManager.h>
#include <SpriteManager.h>
#include <cassert>

//アプリケーション
#include <application/system/CameraManager.h>

using namespace Norm;

void DirectionUI::Initialize() {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerUI");
	//矢印の生成・初期化
	{
		Vector2 centerPos = { param_["directionArrow"]["centerPos"]["x"],param_["directionArrow"]["centerPos"]["y"] };
		thArrow_ = TextureManager::GetInstance()->LoadTexture("directionArrow.png");
		spriteArrow_ = std::make_unique<Sprite>();
		spriteArrow_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("PlayerDirectionArrowUI"), Order::Front1, thArrow_);
		spriteArrow_->SetAnchorPoint({ 0.5f,0.5f });
		spriteArrow_->SetPosition(centerPos);
	}
	//目盛りの生成・初期化
	{
		Vector2 centerPos = { param_["direction"]["centerPos"]["x"],param_["direction"]["centerPos"]["y"] };
		Vector2 textureSize = { param_["direction"]["textureSize"]["x"],param_["direction"]["textureSize"]["y"] };

		thScale_ = TextureManager::GetInstance()->LoadTexture("directionUI.png");
		spriteScale_ = std::make_unique<Sprite>();
		spriteScale_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("PlayerDirectionUI"), Order::Front1, thScale_);
		spriteScale_->SetAnchorPoint({ 0.5f,0.5f });
		spriteScale_->SetPosition(centerPos);
		spriteScale_->SetTextureSize(textureSize);
		spriteScale_->SetSize(textureSize);
	}
}

void DirectionUI::Update() {
	//カメラマネージャーがセットされていなければ警告
	assert(cameraManager_ && "カメラマネージャーをセットしてください");
	//目盛りの回転
	{
		// カメラの現在の回転（Yaw）を取得
		Vector3 cameraRotate = cameraManager_->GetActiveCamera()->worldTransform.GetRotate();

		// 角度を -π～π の範囲に正規化
		cameraRotate.y = MyMath::NormalizeAngle(cameraRotate.y);

		// コンパステクスチャのサイズを取得
		Vector2 textureSize = spriteScale_->GetTextureSize();

		// 1ラジアンあたりのピクセル量
		float pixelsPerRad = 640.0f / pi;

		// 回転角をピクセル単位のオフセットに変換
		float angleOffset = cameraRotate.y * pixelsPerRad;

		// テクスチャ内で「基準にしたい方角」のX座標
		float pivotX = param_["direction"]["pivotX"];

		// pivotX をスプライト中央に合わせるため、
		// テクスチャの半分を引いて左上座標に変換する
		float scrollX = (pivotX - textureSize.x * 0.5f) + angleOffset;

		// UV左上座標を設定
		Vector2 leftTop;
		leftTop.x = scrollX;
		leftTop.y = 0.0f;

		// スプライトのUVスクロールを更新
		spriteScale_->SetTextureLeftTop(leftTop);
	}
}

void DirectionUI::AttachShake(const Norm::Vector2& _shakeOffset) {
	spriteArrow_->SetShakeOffset(_shakeOffset);
	spriteScale_->SetShakeOffset(_shakeOffset);
}

void DirectionUI::AttachBlinking(const Norm::Vector4& _color) {
	spriteArrow_->SetColor(_color);
	spriteScale_->SetColor(_color);
}

void DirectionUI::SetIsDisplay(bool _isDisplay) {
	spriteArrow_->SetIsDisplay(_isDisplay);
	spriteScale_->SetIsDisplay(_isDisplay);
}