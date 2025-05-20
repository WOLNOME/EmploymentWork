#include "BaseCharacter.h"
#include <cassert>

void BaseCharacter::Initialize() {
	//デバッグ用ラインの初期化
	InitDebugLine();
}

void BaseCharacter::Update() {
	//カメラのセットを確認
	assert(camera_ != nullptr && "カメラがセットされていません");

	//オブジェクトの更新
	object3d_->Update();

}

void BaseCharacter::DrawLine() {
	//コライダーのライン描画
	DrawCollisionLine(camera_);

}
