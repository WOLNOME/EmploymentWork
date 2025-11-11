#include "BaseCharacter.h"
#include "CollisionManager.h"
#include "LineManager.h"
#include <Object3dManager.h>
#include <cassert>

void BaseCharacter::Initialize() {
	//丸影の生成
	circleShadow_ = std::make_unique<Object3d>();
	circleShadow_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("CircleShadow"), "circleShadow");
	circleShadow_->SetIsLightProcess(false);
	circleShadow_->worldTransform.scale = { 5.0f,5.0f,5.0f };
}

void BaseCharacter::Update() {
	//前フレーム座標の更新
	prePosition_ = object3d_->worldTransform.translate;

	//丸影の更新処理
	if (circleShadow_) {
		circleShadow_->worldTransform.translate = object3d_->worldTransform.worldTranslate;
		circleShadow_->worldTransform.translate.y = 0.01f;
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

	//不可視にする
	object3d_->SetIsDisplay(false);
	if (circleShadow_) {
		circleShadow_->SetIsDisplay(false);
	}

	//死亡予約時間をセット
	deadTimer_ = remainingSeconds;

	//当たり判定を消滅させる
	SetCollisionAttribute(CollisionAttribute::Nothingness);

	return;
}
