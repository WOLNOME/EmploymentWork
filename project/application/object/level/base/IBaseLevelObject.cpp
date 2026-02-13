#include "IBaseLevelObject.h"
#include <Object3dManager.h>
#include <CollisionManager.h>

using namespace Norm;

void IBaseLevelObject::Initialize(const std::string& _name, const std::string& _filePath, const TransformEuler& _transform) {
	//名前のセット
	name_ = _name;
	//オブジェクトの生成と初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName(_name), _filePath);
	//トランスフォームパラメータのセット
	object3d_->worldTransform.SetTranslate(_transform.translate);
	object3d_->worldTransform.SetRotate(_transform.rotate);
	object3d_->worldTransform.SetScale(_transform.scale);

	//当たり判定の生成・初期化
	collider_ = std::make_unique<LevelObjectCollider>(this);
	collider_->SetWorldTransform(&object3d_->worldTransform);
	collider_->SetCollisionAttribute(CollisionAttribute::Nothingness);

}

void IBaseLevelObject::Update() {

	//ツリーオブジェクトの転倒アクション
	TreeInvertProcess();

}

void IBaseLevelObject::DebugWithImGui() {
#ifdef _DEBUG

	//コライダーのデバッグ処理
	if (collider_) {
		collider_->Debug();
	}

#endif // _DEBUG
}

void IBaseLevelObject::SetCollisionInfo(const Vector3& _center, const Vector3& _size) {
	//コリジョンの有効化
	collider_->SetOffset(_center / 2.0f);
	collider_->SetOBBSize(_size / 2.0f);
	//当たり判定の属性を設定
	collider_->SetCollisionAttribute(CollisionAttribute::Wall);
}

void IBaseLevelObject::TreeInvertProcess() {
	if (isAction_) {
		//アクション中ならタイマーを進める
		timer_ += kDeltaTime;
		timer_ = std::min(timer_, time_);

		//新回転
		Vector3 newRotate = object3d_->worldTransform.GetRotate();

		//Lerpで倒れる方向に回転
		newRotate.x = MyMath::Lerp(0.0f, invertDirection_.z * -(pi / 2.0f), MyMath::EaseInOutSine(timer_ / time_));
		newRotate.z = MyMath::Lerp(0.0f, invertDirection_.x * (pi / 2.0f), MyMath::EaseInOutSine(timer_ / time_));

		//新回転をセット
		object3d_->worldTransform.SetRotate(newRotate);

		//タイマーが経過したらアクションを終了
		if (timer_ >= time_) {
			isAction_ = false;
			timer_ = 0.0f;
		}
	}
}
