#pragma once
#pragma once
#include "MyMath.h"
#include <Sprite.h>
#include <wrl.h>
#include <cstdint>
#include <memory>

class SceneTransitionAnimation {
public:
	//遷移の状態
	enum class State {
		NONE,
		UPDATE_IN,
		END_IN,
		UPDATE_OUT,
		END_OUT,
		END_ALL,
	};
	//遷移の種類
	enum class Type {
		NONE,
		FADE,
		SLIDEUP,
		SLIDEDOWN,
	};
	//遷移オプション
	enum class Option {
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
	State GetState() const { return state_; }
	bool IsTransitioning() const { return isTransitioning_; }
	//セッター
	void SetType(Type _in, Type _out) { inType_ = _in; outType_ = _out; }
	void SetOption(Option _option) { option_ = _option; }
	void SetTime(float _time) { time_ = _time; }
	void SetTexture(uint32_t _textureHandle);

private:
	//スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	//遷移の状態
	State state_;
	//遷移の種類
	Type inType_;
	Type outType_;
	//遷移のオプション
	Option option_;
	//時間
	float time_;
	float timer_;
	//遷移中フラグ
	bool isTransitioning_ = false;

};
