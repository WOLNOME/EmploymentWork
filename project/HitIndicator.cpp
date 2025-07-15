#include "HitIndicator.h"
#include <WinApp.h>
#include <TextureManager.h>
#include <SpriteManager.h>
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

		//スプライトの回転を決める
		float rotation = 0.0f;
		//使用する変数
		Vector3 p=

			被弾インジケーターの実装

			HitIndicatorクラスを作る
			情報をまとめた構造体を作る。

			構造体の中身は
			スプライト
			現在時間
			ヒットした弾の座標
			有効かどうかのフラグ

			max時間 = 3はメンバ変数に持たせる

			この構造体をstd::array<, 10>で宣言し、
			Initialize()で生成・初期化
			Updateでの処理は
			全ての配列を回し、その中の有効な要素のみ
			現在時間をdeltaTimeで引いていく。
			現在時間が0以下になったらスプライトを非表示、透明度を戻し、有効フラグを無効化して次の要素へ。

			スプライトのα値を現在時間 / maxTimeでだんだん透明にしていく。
			また、プレイヤーの正面ベクトルとヒットした弾の座標へのプレイヤーの座標を結ぶベクトルの角度によってスプライトのrotateを変化。

			RegistIndicator関数を追加
			引数にヒットしたときの弾の座標を入れる
			要素を回して無効フラグのところに
			スプライトを有効化
			現在時間 = max時間
			ヒットした弾の座標をセット
			有効フラグを有効化にする。








	}
}

void HitIndicator::RegistIndicator(const Vector3& _hitPosition) {
}
