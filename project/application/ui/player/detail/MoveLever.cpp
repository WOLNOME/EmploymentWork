#include "MoveLever.h"
#include <TextureManager.h>
#include <SpriteManager.h>

//アプリケーション
#include <application/object/character/player/Player.h>

using namespace Norm;

void MoveLever::Initialize() {
	//制御目盛りUIのスプライトの初期化
	textureHandles_[(int)Type::kControlScale] = TextureManager::GetInstance()->LoadTexture("controlScale.png");
	sprites_[(int)Type::kControlScale] = std::make_unique<Sprite>();
	sprites_[(int)Type::kControlScale]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("controlScaleUI"), Order::Front2, textureHandles_[(int)Type::kControlScale]);
	sprites_[(int)Type::kControlScale]->SetAnchorPoint({ 0.5f,0.5f });
	sprites_[(int)Type::kControlScale]->SetPosition({ 1100.0f,640.0f });
	//レバーUIのスプライトの初期化
	textureHandles_[(int)Type::kLever] = TextureManager::GetInstance()->LoadTexture("lever.png");
	sprites_[(int)Type::kLever] = std::make_unique<Sprite>();
	sprites_[(int)Type::kLever]->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("leverUI"), Order::Front3, textureHandles_[(int)Type::kLever]);
	sprites_[(int)Type::kLever]->SetAnchorPoint({ 0.5f,0.5f });
	sprites_[(int)Type::kLever]->SetPosition({ 1100.0f,640.0f });
}

void MoveLever::Update() {
	//プレイヤーの速度を参照してレバーの位置を決める
	const float maxHeight = 580.0f;
	const float minHeight = 700.0f;

	//W,Sに対してゲージを変動させる
	if (input_->PushKey(DIK_W) || (input_->GetLStickDir().y > 0.0f)) {
		//半分より小さいなら
		if (gauge_ < kMaxGauge_ / 2.0f) {
			gauge_ += 2;
		}
		else {
			gauge_++;
		}
	}
	if (input_->PushKey(DIK_S) || (input_->GetLStickDir().y < 0.0f)) {
		//半分より大きいなら
		if (gauge_ > kMaxGauge_ / 2.0f) {
			gauge_ -= 2;
		}
		else {
			gauge_--;
		}
	}

	//何も入力されていない場合は1500になるようにする
	if (!input_->PushKey(DIK_W) &&
		!input_->PushKey(DIK_S) &&
		(input_->GetLStickDir().y == 0.0f)) {
		//半分より大きいなら
		if (gauge_ > kMaxGauge_ / 2.0f) {
			gauge_ -= 2;
		}
		//半分より小さいなら
		else if (gauge_ < kMaxGauge_ / 2.0f) {
			gauge_ += 2;
		}
	}

	//ゲージをクランプする
	gauge_ = std::clamp<int>(gauge_, 0, kMaxGauge_);

	float leverHeight = MyMath::Lerp(minHeight, maxHeight, (float)gauge_ / kMaxGauge_);

	//レバーの座標を決める
	sprites_[(int)Type::kLever]->SetPosition({ 1100.0f,leverHeight });
}

void MoveLever::AttachShake(const Vector2& _shakeOffset) {
	//スプライトにシェイクを適用する
	for (int i = 0; i < 2; i++) {
		sprites_[i]->SetShakeOffset(_shakeOffset);
	}
}

void MoveLever::AttachBlinking(const Vector4& _color) {
	//スプライトに点滅を適用する
	for (int i = 0; i < 2; i++) {
		sprites_[i]->SetColor(_color);
	}
}