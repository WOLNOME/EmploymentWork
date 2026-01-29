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
	circleShadow_->worldTransform.translate = { FLT_MAX,FLT_MAX ,FLT_MAX };
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
}

void BaseCharacter::DebugWithImGui() {
#ifdef _DEBUG
	//アクティブでないなら
	if (state_!=State::kActive)
		return;

	Collider::DebugWithImGui();
#endif //_DEBUG
}

void BaseCharacter::SetState(const State& _state) {
	//値のセット
	state_ = _state;
	//状態ごとの処理
	switch (state_) {
	case BaseCharacter::State::kIdle:
		//座標を遥か遠くにセット
		object3d_->worldTransform.translate = { FLT_MAX,FLT_MAX ,FLT_MAX };
		if (circleShadow_) {
			circleShadow_->worldTransform.translate = { FLT_MAX,FLT_MAX ,FLT_MAX };
		}
		//不可視にする
		object3d_->SetIsDisplay(false);
		if (circleShadow_) {
			circleShadow_->SetIsDisplay(false);
		}
		//当たり判定を消滅
		SetCollisionAttribute(CollisionAttribute::Nothingness);
		break;
	case BaseCharacter::State::kActive:
		//特に何もしない
		break;
	case BaseCharacter::State::kAsphyxia:
		//不可視にする
		object3d_->SetIsDisplay(false);
		if (circleShadow_) {
			circleShadow_->SetIsDisplay(false);
		}
		//当たり判定を消滅
		SetCollisionAttribute(CollisionAttribute::Nothingness);
		break;
	default:
		break;
	}

}
