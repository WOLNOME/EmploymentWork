#pragma once
#include "MyMath.h"
#include <Sprite.h>
#include <wrl.h>
#include <cstdint>
#include <memory>

class SceneTransitionAnimation {
public:
	//遷移の状態
	enum class TransitionState {
		NONE,
		UPDATE_IN,
		END_IN,
		UPDATE_OUT,
		END_OUT,
		END_ALL,
	};
	//遷移の種類
	enum class TransitionType {
		NONE,
		FADE,
		SLIDEUP,
		SLIDEDOWN,
	};
	//遷移オプション
	enum class TransitionOption {
		NONE,
		SHAKE,
	};
public:
	SceneTransitionAnimation();
	~SceneTransitionAnimation();
	void Initialize();
	void Update();

	//遷移処理全般
	void StartTransition();
	void UpdateIn();
	void EndIn();
	void UpdateOut();
	void EndOut();
	void EndAll();

	//ゲッター
	TransitionState GetState() const { return state_; }
	bool IsTransitioning() const { return isTransitioning_; }
	//セッター
	void SetTransitionType(TransitionType _in, TransitionType _out) { inType_ = _in; outType_ = _out; }
	void SetTransitionOption(TransitionOption _in, TransitionOption _out) { inOption_ = _in; outOption_ = _out; }
	void SetTime(float _time) { time_ = _time; }
	void SetTexture(uint32_t _textureHandle);

private:
	//スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	//遷移の状態
	TransitionState state_;
	//遷移の種類
	TransitionType inType_;
	TransitionType outType_;
	//遷移のオプション
	TransitionOption inOption_;
	TransitionOption outOption_;
	//時間
	float time_;
	float timer_;
	//遷移中フラグ
	bool isTransitioning_;

};

