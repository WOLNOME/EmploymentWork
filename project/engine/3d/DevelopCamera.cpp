#include "DevelopCamera.h"
#include "ImGuiManager.h"
#include <algorithm>
#include <numbers>

void DevelopCamera::Initialize() {
	//基盤の初期化
	BaseCamera::Initialize();
	//インプット
	input_ = Input::GetInstance();

}

void DevelopCamera::Update() {

	////開発用カメラのマウス操作処理
	//スクロールで前進後退
	standardPosition += GetForwardDirection() * (input_->GetMouseScrollCount() * 1.3f);
	//ドラッグアンドドロップでカメラの向きを変える
	if (input_->PushMouseButton(MouseButton::RightButton)) {
		//マウスの移動幅
		Vector2 moveValue = input_->GetMousePosition();
		//デッドゾーン
		float deadZone = 2.0f;
		if (moveValue.Length() > deadZone) {
			worldTransform.rotate.x += moveValue.y * 0.001f;
			worldTransform.rotate.y += moveValue.x * 0.001f;
		}

		//WASDでカメラ移動
		if (input_->PushKey(DIK_W)) {
			standardPosition += GetForwardDirection() * 0.1f;
		}
		if (input_->PushKey(DIK_A)) {
			standardPosition += GetLeftDirection() * 0.1f;
		}
		if (input_->PushKey(DIK_S)) {
			standardPosition += GetBackDirection() * 0.1f;
		}
		if (input_->PushKey(DIK_D)) {
			standardPosition += GetRightDirection() * 0.1f;
		}

	}
	//カメラの回転制限
	const float maxPitch = (std::numbers::pi_v<float> / 2.0f) - 0.01f;
	worldTransform.rotate.x = std::clamp(worldTransform.rotate.x, -maxPitch, maxPitch);

	//ベースの更新
	BaseCamera::Update();

}

void DevelopCamera::DebugWithImGui() {
#ifdef _DEBUG

	ImGui::Begin("DevelopCamera");
	ImGui::DragFloat3("Translate", &standardPosition.x, 0.01f);
	ImGui::DragFloat3("Rotate", &worldTransform.rotate.x, 0.01f);
	ImGui::End();

	ImGui::Begin("mouse");
	if (input_->PushMouseButton(MouseButton::LeftButton)) {
		ImGui::Text("LEFT");
	}
	if (input_->PushMouseButton(MouseButton::MiddleButton)) {
		ImGui::Text("MIDDLE");
	}
	if (input_->PushMouseButton(MouseButton::RightButton)) {
		ImGui::Text("RIGHT");
	}
	ImGui::Text("ScrollValue : %f", input_->GetMouseScrollCount());
	ImGui::End();

#endif // _DEBUG
}
