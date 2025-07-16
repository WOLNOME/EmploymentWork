#pragma once
#include <Sprite.h>
#include <memory>

class ReticleUI {
public:
	//初期化
	void Initialize();

private:
	uint32_t textureHandle_ = 0u;
	std::unique_ptr<Sprite> sprite_ = nullptr;
};

