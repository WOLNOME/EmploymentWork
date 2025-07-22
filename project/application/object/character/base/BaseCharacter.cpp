#include "BaseCharacter.h"
#include "CollisionManager.h"
#include "LineManager.h"
#include <cassert>

void BaseCharacter::Update() {
	//前フレーム座標の更新
	prePosition_ = object3d_->worldTransform.translate;

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
			object3d_->worldTransform.translate = { FLT_MAX,FLT_MAX ,FLT_MAX };
		}
	}

	//死亡しているのなら描画しない
	if (deadTimer_ > 0.0f || isDead_)
		object3d_->SetIsDisplay(false);

}

void BaseCharacter::DebugWithImGui() {
#ifdef _DEBUG
	//死亡していたらreturn
	if (isDead_ || deadTimer_ > 0.0f) return;

	Collider::DebugWithImGui();
#endif //_DEBUG
}

void BaseCharacter::SetDeadTimer(float remainingSeconds) {
	//もし予約時間が決まっていたらreturn
	if (deadTimer_ > 0.0f) return;

	//死亡予約時間をセット
	deadTimer_ = remainingSeconds;

	//当たり判定を消滅させる
	SetCollisionAttribute(CollisionAttribute::Nothingness);

	return;
}
