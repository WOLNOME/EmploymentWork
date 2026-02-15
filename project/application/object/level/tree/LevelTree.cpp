#include "LevelTree.h"

using namespace Norm;

void LevelTree::Initialize(const std::string& _name, const std::string& _filePath) {
	//基底クラスの初期化
	IBaseLevelObject::Initialize(_name, _filePath);

}

void LevelTree::Update() {
	//基底クラスの更新
	IBaseLevelObject::Update();

	//倒木処理
	TreeInvertProcess();
}

void LevelTree::DebugWithImGui() {
	//基底クラスのデバッグ
	IBaseLevelObject::DebugWithImGui();
}

void LevelTree::TreeInvertProcess() {
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