#pragma once
#include <Sprite.h>
#include <Vector3.h>
#include <memory>
#include <array>

class Player;
class HitIndicator {
private://構造体
	struct IndicatorInfo {
		uint32_t textureHandle = 0u;
		std::unique_ptr<Sprite> sprite_ = nullptr;
		float currentTime_;
		Vector3 hitPosition_;
		bool isActive_;
	};
public:
	void Initialize();
	void Update();

	void RegistIndicator(const Vector3& _hitPosition);

public://setter
	void SetPlayer(Player* _player) { player_ = _player; }

private://借用インスタンス
	Player* player_ = nullptr;
private:
	static const int kNumIndicators_ = 10;
	std::array<IndicatorInfo, kNumIndicators_> indicators_;

	const float maxActiveTime_ = 3.0f;
};

