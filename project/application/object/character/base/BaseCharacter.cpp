#include "BaseCharacter.h"
#include "CollisionManager.h"
#include "LineManager.h"
#include <float.h>
#include <cassert>

void BaseCharacter::Initialize() {
	//前フレーム座標を初期化
	prePosition_ = { FLT_MAX,FLT_MAX ,FLT_MAX };
}

void BaseCharacter::Update() {
	//前フレーム座標の更新
	if (prePosition_.x == FLT_MAX) {
		//初期化してから最初のフレーム→ローカル座標に合わせる
		if (!object3d_->worldTransform.parent) {
			//親子付けしていなかったら
			prePosition_ = object3d_->worldTransform.translate;
		}
		else {
			//親子付けしていたら
			Matrix4x4 matWorld = MyMath::MakeAffineMatrix(object3d_->worldTransform.scale, object3d_->worldTransform.rotate, object3d_->worldTransform.translate);
			matWorld = matWorld * object3d_->worldTransform.parent->matWorld;
			prePosition_.x = matWorld.m[3][0];
			prePosition_.y = matWorld.m[3][1];
			prePosition_.z = matWorld.m[3][2];
		}
	}
	else {
		//2フレーム以降
		prePosition_ = object3d_->worldTransform.worldTranslate;
	}

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
	if(deadTimer_>0.0f||isDead_)
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
	return;
}
