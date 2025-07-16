#include "HitIndicator.h"
#include <WinApp.h>
#include <TextureManager.h>
#include <SpriteManager.h>
#include <GameCamera.h>
#include <MyMath.h>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>

void HitIndicator::Initialize() {
	//メンバ変数の初期化
	for (int i = 0; i < kNumIndicators_; i++) {
		//テクスチャ
		indicators_[i].textureHandle = TextureManager::GetInstance()->LoadTexture("HitIndicator.png");
		//スプライト
		indicators_[i].sprite_ = std::make_unique<Sprite>();
		indicators_[i].sprite_->Initialize(SpriteManager::GetInstance()->GenerateName("hitIndicator"),Sprite::Order::Front2,indicators_[i].textureHandle);
		indicators_[i].sprite_->SetAnchorPoint({ 0.5f,0.5f });
		indicators_[i].sprite_->SetPosition({ WinApp::GetInstance()->kClientWidth / 2.0f,WinApp::GetInstance()->kClientHeight / 2.0f });
		indicators_[i].sprite_->SetIsDisplay(false);
		//現在時間
		indicators_[i].currentTime_ = 0.0f;
		//衝突座標
		indicators_[i].hitPosition_ = {};
		//有効フラグ
		indicators_[i].isActive_ = false;
	}
}

void HitIndicator::Update() {
	//プレイヤーがセットされていなかったら警告
	if (!player_) {
		assert(0 && "プレイヤーがセットされていません");
	}
	//ゲームカメラがセットされていなかったら警告
	if (!gameCamera_) {
		assert(0 && "ゲームカメラがセットされていません");
	}

	//インジケーターの更新処理
	for (int i = 0; i < kNumIndicators_; i++) {
		//有効でないなら次へ
		if (!indicators_[i].isActive_) continue;

		//寿命を減らす
		indicators_[i].currentTime_ -= kDeltaTime;
		//寿命に達したら
		if (indicators_[i].currentTime_ <= 0.0f) {
			//現在時間を0に
			indicators_[i].currentTime_ = 0.0f;
			//スプライトを非表示
			indicators_[i].sprite_->SetIsDisplay(false);
			//透明度を元に戻す
			indicators_[i].sprite_->SetColor({ 1,1,1,1 });
			//ヒットポジションをリセット
			indicators_[i].hitPosition_ = {};
			//有効フラグを無効化
			indicators_[i].isActive_ = false;
			
			//次の要素へ
			continue;
		}

		//スプライトのα値を決める
		float alpha = float(indicators_[i].currentTime_ / maxActiveTime_);
		indicators_[i].sprite_->SetColor({ 1,1,1,alpha });

		//使用する変数
		Vector3 p = player_->GetWorldTransform().translate;	//プレイヤーの座標
		Vector3 v1 = {
			std::sinf(gameCamera_->worldTransform.rotate.y),
			0.0f,
			std::cosf(gameCamera_->worldTransform.rotate.y)
		};	//カメラの向き
		Vector3 v2 = indicators_[i].hitPosition_ - p;	//目標位置への方向
		v2.y = 0.0f;
		v2.Normalize();

		//外積と内積から角度を求める
		float dot = v1.Dot(v2);
		float cross = v1.Cross(v2).y;

		//atan2で回転角を得る
		float rotation = std::atan2f(cross, dot);

		//スプライトに適用
		indicators_[i].sprite_->SetRotation(rotation);

	}
}

void HitIndicator::RegistIndicator(const Vector3& _hitPosition) {
	//全てのインジケーターを回す
	for (int i = 0; i < kNumIndicators_; i++) {
		//インジケーターが有効なら次へ
		if (indicators_[i].isActive_) {
			continue;
		}

		//衝突点を入れる
		indicators_[i].hitPosition_ = _hitPosition;
		//スプライトを表示
		indicators_[i].sprite_->SetIsDisplay(true);
		//現在時間をセット
		indicators_[i].currentTime_ = maxActiveTime_;
		//この要素を有効に
		indicators_[i].isActive_ = true;

		break;
	}
}
