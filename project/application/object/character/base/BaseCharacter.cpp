#include "BaseCharacter.h"
#include "LineManager.h"
#include "ICollider.h"
#include <Object3dManager.h>
#include <cassert>

using namespace Norm;

BaseCharacter::BaseCharacter() {
}

BaseCharacter::~BaseCharacter() {
}

void BaseCharacter::Initialize() {
	//丸影の生成
	circleShadow_ = std::make_unique<Object3d>();
	circleShadow_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("CircleShadow"), "circleShadow");
	circleShadow_->SetIsLightProcess(false);
	circleShadow_->worldTransform.SetTranslate({ FLT_MAX,FLT_MAX ,FLT_MAX });
	circleShadow_->worldTransform.SetScale({ 5.0f,5.0f,5.0f });
	circleShadow_->SetIsDisplay(false);
}

void BaseCharacter::Update() {
	//前フレーム座標の更新
	prePosition_ = object3d_->worldTransform.GetTranslate();

	//丸影の更新処理
	circleShadow_->worldTransform.SetTranslate(object3d_->worldTransform.GetTranslate());
	Vector3 translate = circleShadow_->worldTransform.GetTranslate();
	translate.y = 0.01f;
	circleShadow_->worldTransform.SetTranslate(translate);
}

void BaseCharacter::DebugWithImGui() {
#ifdef _DEBUG
	//アクティブでないならreturn
	if (state_ != State::kActive)
		return;

	if (collider_) {
		collider_->Debug();
	}
#endif //_DEBUG
}

void BaseCharacter::SetState(const State& _state) {
	//値のセット
	state_ = _state;
	//状態ごとの処理
	switch (state_) {
	case BaseCharacter::State::kIdle:
		//座標を遥か遠くにセット
		object3d_->worldTransform.SetTranslate({ FLT_MAX,FLT_MAX ,FLT_MAX });
		if (circleShadow_) {
			circleShadow_->worldTransform.SetTranslate({ FLT_MAX,FLT_MAX ,FLT_MAX });
		}
		//不可視にする
		object3d_->SetIsDisplay(false);
		if (circleShadow_) {
			circleShadow_->SetIsDisplay(false);
		}
		//当たり判定を消滅
		if (collider_) {
			collider_->SetCollisionAttribute(CollisionAttribute::Nothingness);
		}
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
		if (collider_) {
			collider_->SetCollisionAttribute(CollisionAttribute::Nothingness);
		}
		break;
	default:
		break;
	}

}
