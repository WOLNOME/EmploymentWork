#pragma once
#include "BaseCamera.h"
#include "Input.h"
#include "Vector2.h"

/// <summary>
/// 開発用カメラの管理を行うクラス
/// </summary>
class DevelopCamera : public BaseCamera {
public:
	DevelopCamera() : BaseCamera() {}
	virtual ~DevelopCamera() = default;
	//初期化
	void Initialize() override;
	//更新
	void Update() override;

	//ImGui操作
	void DebugWithImGui();

private:
	Input* input_ = nullptr;

	Vector2 start = { 0.0f,0.0f };

};
