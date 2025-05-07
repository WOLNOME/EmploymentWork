#pragma once
#include "BaseCamera.h"
#include "WorldTransform.h"
#include "Object3d.h"
#include <memory>

class Skydome {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(BaseCamera& camera_);
private:
	WorldTransform worldTransform_;
	int32_t textureHandle_ = EOF;
	std::unique_ptr<Object3d> object3d_ = nullptr;

};

