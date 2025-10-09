#include "SceneTransitionAnimation.h"
#include "WinApp.h"
#include <SpriteManager.h>
#include  <TextureManager.h>
#include <cassert>

SceneTransitionAnimation::SceneTransitionAnimation()
	: state_(TransitionState::NONE), inType_(TransitionType::NONE), outType_(TransitionType::NONE), inOption_(TransitionOption::NONE), outOption_(TransitionOption::NONE), time_(0), timer_(0) {
}

SceneTransitionAnimation::~SceneTransitionAnimation() {
}

void SceneTransitionAnimation::Initialize() {
	//スプライト生成
	sprite_ = std::make_unique<Sprite>();
	uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("black.png");
	sprite_->Initialize(SpriteManager::GetInstance()->GenerateName("transitionBack"), Sprite::Order::SceneTransition, textureHandle);
	sprite_->SetIsDisplay(false);

	//変数の初期化
	state_ = TransitionState::NONE;
	inType_ = TransitionType::NONE;
	outType_ = TransitionType::NONE;
	inOption_ = TransitionOption::NONE;
	outOption_ = TransitionOption::NONE;
	time_ = 0.0f;
	timer_ = 0.0f;
}

void SceneTransitionAnimation::Update() {
}

void SceneTransitionAnimation::StartTransition() {
	//必要な変数の確認
	if (inType_ == TransitionType::NONE || outType_ == TransitionType::NONE) {
		assert(0 && "遷移の種類が設定されていません");
	}
	if (time_ == 0) {
		assert(0 && "時間が設定されていません");
	}

	//イン開始
	state_ = TransitionState::UPDATE_IN;
	//スプライトを表示
	sprite_->SetIsDisplay(true);

	//タイプごとの初期化
	//switch (inType_) {
	//case SceneTransitionAnimation::TransitionType::FADE:
	//	//座標を画面中央に
	//	sprite_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
	//	//アンカーポイント
	//	sprite_->SetAnchorPoint({ 0.5f,0.5f });
	//	//オプションがシェイクなら
	//	if (inOption_ == TransitionOption::SHAKE) {
	//		//サイズを少し大きく
	//	}
	//	break;
	//case SceneTransitionAnimation::TransitionType::SLIDEUP:
	//	break;
	//case SceneTransitionAnimation::TransitionType::SLIDEDOWN:
	//	break;
	//};
}

void SceneTransitionAnimation::UpdateIn() {
	switch (inType_) {
	case SceneTransitionAnimation::TransitionType::FADE:
		//フェードイン処理
		if (state_ == TransitionState::UPDATE_IN) {
			//時間が達したら
			if (timer_ >= time_) {
				//フェードイン終了
				state_ = TransitionState::END_IN;
			}
			else {
				//時間を進める
				timer_ += kDeltaTime;
				//透明度を計算
				float alpha = MyMath::Lerp(0.0f, 1.0f, static_cast<float>(timer_) / static_cast<float>(time_));
				//スプライトの透明度を設定
				Vector4 color = sprite_->GetColor();
				color.w = alpha;
				sprite_->SetColor(color);
			}
		}
		break;
	default:
		break;
	}

}

void SceneTransitionAnimation::EndIn() {
	switch (inType_) {
	case SceneTransitionAnimation::TransitionType::FADE:
		//フェードイン終了
		if (state_ == TransitionState::END_IN) {
			//フェードアウト開始
			state_ = TransitionState::UPDATE_OUT;
			//フレームをリセット
			timer_ = 0.0f;
		}
		break;
	default:
		break;
	}
}

void SceneTransitionAnimation::UpdateOut() {
	switch (outType_) {
	case SceneTransitionAnimation::TransitionType::FADE:
		//フェードアウト処理
		if (state_ == TransitionState::UPDATE_OUT) {
			//時間が達したら
			if (timer_ >= time_) {
				//フェードアウト終了
				state_ = TransitionState::END_OUT;
			}
			else {
				//時間を進める
				timer_ += kDeltaTime;
				//透明度を計算
				float alpha = MyMath::Lerp(1.0f, 0.0f, static_cast<float>(timer_) / static_cast<float>(time_));
				//スプライトの透明度を設定
				Vector4 color = sprite_->GetColor();
				color.w = alpha;
				sprite_->SetColor(color);
			}
		}
		break;
	default:
		break;
	}
}

void SceneTransitionAnimation::EndOut() {
	switch (outType_) {
	case SceneTransitionAnimation::TransitionType::FADE:
		//フェードアウト終了
		if (state_ == TransitionState::END_OUT) {
			//遷移終了
			state_ = TransitionState::END_ALL;
		}
		break;
	default:
		break;
	}
}

void SceneTransitionAnimation::EndAll() {
	//遷移終了
	state_ = TransitionState::NONE;
	inType_ = TransitionType::NONE;
	outType_ = TransitionType::NONE;
	inOption_ = TransitionOption::NONE;
	outOption_ = TransitionOption::NONE;
	time_ = 0.0f;
	timer_ = 0.0f;
	//スプライトを非表示
	sprite_->SetIsDisplay(false);
}

void SceneTransitionAnimation::SetTexture(uint32_t _textureHandle) {
	//もし0なら何もしない
	if (_textureHandle == 0u) return;

	sprite_->SetTexture(_textureHandle);

	//テクスチャのサイズを画面サイズ(1280x720)に合わせる
	//sprite_->SetSize({ 1200.0f,700.0f });
}
