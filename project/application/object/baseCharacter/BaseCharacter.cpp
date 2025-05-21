#include "BaseCharacter.h"
#include "CollisionManager.h"
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

	//当たり判定を登録
	CollisionManager::GetInstance()->SetColliders(this);

	//死亡予約時の処理
	if (deadTimer_ > 0.0f) {
		//死亡予約時間を減算
		deadTimer_ -= kDeltaTime;
		//死亡予約時間が0以下になったら死亡フラグを立てる
		if (deadTimer_ <= 0.0f) {
			isDead_ = true;
			deadTimer_ = 0.0f;
		}
	}

}

void BaseCharacter::DrawLine() {
	//コライダーのライン描画
	DrawCollisionLine(camera_);

}

void BaseCharacter::SetDeadTimer(float remainingSeconds) {
	//もし予約時間が決まっていたらreturn
	if (deadTimer_ > 0.0f) return;

	//死亡予約時間をセット
	deadTimer_ = remainingSeconds;
	return;
}
