#pragma once
#include "BaseCamera.h"
#include "Object3d.h"
#include <memory>

class Skydome {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

private:
	int32_t textureHandle_ = EOF;
	std::unique_ptr<Object3d> object3d_ = nullptr;

};

