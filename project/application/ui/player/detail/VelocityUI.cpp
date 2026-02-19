#include "VelocityUI.h"
#include <TextTextureManager.h>
#include <TextureManager.h>
#include <SpriteManager.h>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>

using namespace Norm;

void VelocityUI::Initialize() {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/playerUI");
	playerParam_ = JsonUtil::GetJsonData("Resources/parameters/player");

	//速度計UIのスプライトの初期化
	{
		Vector2 centerPos = { param_["velocityUI"]["centerPos"]["x"],param_["velocityUI"]["centerPos"]["y"] };
		thSpeedMeter_ = TextureManager::GetInstance()->LoadTexture("speedMeter.png");
		spriteSpeedMeter_ = std::make_unique<Sprite>();
		spriteSpeedMeter_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("speedMeterUI"), Order::Front2, thSpeedMeter_);
		spriteSpeedMeter_->SetAnchorPoint({ 0.5f,0.5f });
		spriteSpeedMeter_->SetPosition(centerPos);
	}
	//針のスプライトの初期化
	{
		Vector2 centerPos = { param_["velocityUI"]["centerPos"]["x"],param_["velocityUI"]["centerPos"]["y"] };
		centerPos.x += param_["velocityUI"]["needleOffset"]["x"];
		centerPos.y += param_["velocityUI"]["needleOffset"]["y"];
		thNeedle_ = TextureManager::GetInstance()->LoadTexture("needle.png");
		spriteNeedle_ = std::make_unique<Sprite>();
		spriteNeedle_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("needleUI"), Order::Front3, thNeedle_);
		spriteNeedle_->SetAnchorPoint({ 0.5f,0.5f });
		spriteNeedle_->SetPosition(centerPos);
	}
	//数値テキストの初期化
	{
		Vector2 centerPos = { param_["velocityUI"]["centerPos"]["x"],param_["velocityUI"]["centerPos"]["y"] };
		centerPos.x += param_["velocityUI"]["numTextOffset"]["x"];
		centerPos.y += param_["velocityUI"]["numTextOffset"]["y"];
		TextParam numTextParam = {
			.text = L"",
			.font = Font::UDDegitalN_B,
			.fontStyle = FontStyle::Normal,
			.size = 32.0f,
			.color = Vector4(0.0549f,0.7490f,0.4745f,1.0f),
		};
		thNumText_ = TextTextureManager::GetInstance()->LoadTextTexture(numTextParam);
		spriteNumText_ = std::make_unique<Sprite>();
		spriteNumText_->Initialize(TextTag{}, SpriteManager::GetInstance()->GenerateName("velocityNumText"), Order::Front3);
		spriteNumText_->SetAnchorPoint({ 0.5f,0.5f });
		spriteNumText_->SetPosition(centerPos);
		spriteNumText_->SetTexture(thNumText_);
	}
}

void VelocityUI::Update() {
	//プレイヤーがセットされていなければ警告
	assert(player_ != nullptr && "プレイヤーがセットされていません。");

	//針を回転させる処理
	{
		//プレイヤーの最大速度
		float maxSpeed = playerParam_["maxSpeed"].get<float>();
		//針の回転最大値(90°)
		float maxRotation = pi * 0.5f;
		//針の回転最小値(-90°)
		float minRotation = -pi * 0.5f;
		//プレイヤーの速度を車体の向きに合わせて正負をつける
		//車体の前後軸にvelocityを分解する。
		Vector3 velocity = player_->GetVelocity();
		Vector3 vehicleDir = {
			std::sinf(player_->GetWorldTransform().GetRotate().y),
			0.0f,
			std::cosf(player_->GetWorldTransform().GetRotate().y)
		};
		vehicleDir.Normalize();
		// 前方向成分（＋なら前進、−なら後退）
		float forwardSpeed = MyMath::Dot(velocity, vehicleDir);

		//針の回転を計算
		float speedRatio = forwardSpeed / maxSpeed;
		speedRatio = std::clamp(speedRatio, -1.0f, 1.0f);
		float rotation = 0.0f;
		if (speedRatio >= 0.0f) {
			rotation = speedRatio * maxRotation;
		}
		else {
			rotation = speedRatio * (-minRotation);
		}

		//回転を適用
		spriteNeedle_->SetRotation(rotation);

		//数値テキストを更新
		TextTextureManager::GetInstance()->EditTextString(thNumText_, L"{:.1f}km/h", forwardSpeed);
	}

}

void VelocityUI::AttachShake(const Vector2& _shakeOffset) {
	spriteSpeedMeter_->SetShakeOffset(_shakeOffset);
	spriteNeedle_->SetShakeOffset(_shakeOffset);
	spriteNumText_->SetShakeOffset(_shakeOffset);
}

void VelocityUI::AttachBlinking(const Vector4& _color) {
	spriteSpeedMeter_->SetColor(_color);
	spriteNeedle_->SetColor(_color);
	spriteNumText_->SetColor(_color);
}