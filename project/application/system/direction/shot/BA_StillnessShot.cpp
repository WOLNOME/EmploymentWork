#include "BA_StillnessShot.h"
#include <BaseCamera.h>

using namespace Norm;

BA_StillnessShot::BA_StillnessShot(int _index, Norm::TransformEuler _startCameraTransform, Norm::TransformEuler _endCameraTransform, float _duration, Norm::BlackBoard* _blackBoard) : IShotBase(_index, _startCameraTransform, _endCameraTransform, _duration, _blackBoard) {
}

BA_StillnessShot::~BA_StillnessShot() {
}

void BA_StillnessShot::Initialize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");

	//カメラの座標、回転を初期状態に合わせる
	camera->worldTransform.SetTranslate(startCameraTransform_.translate);
	camera->worldTransform.SetRotate(startCameraTransform_.rotate);

	//現在時間を初期化
	elapsedTimer_ = duration_;


}

void BA_StillnessShot::Update() {
	//時間を進める
	elapsedTimer_ -= kDeltaTime;

	//もしもタイマーが時間に達したら
	if (elapsedTimer_ < 0.0f) {
		//finishを出力
		shotResult_ = ShotResult::Finish;

		return;
	}
	else {
		//runningを出力
		shotResult_ = ShotResult::Running;
	}
}

void BA_StillnessShot::Finalize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");

	//カメラの座標、回転を最終結果に合わせる
	camera->worldTransform.SetTranslate(endCameraTransform_.translate);
	camera->worldTransform.SetRotate(endCameraTransform_.rotate);
}

void BA_StillnessShot::Debug() {
}
