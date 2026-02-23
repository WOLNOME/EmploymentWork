#include "BA_ShowSealedBoxShot.h"
#include <BaseCamera.h>

using namespace Norm;

BA_ShowSealedBoxShot::BA_ShowSealedBoxShot(int _index, Norm::TransformEuler _startCameraTransform, Norm::TransformEuler _endCameraTransform, float _duration, Norm::BlackBoard* _blackBoard) : IShotBase(_index, _startCameraTransform, _endCameraTransform, _duration, _blackBoard) {
}

BA_ShowSealedBoxShot::~BA_ShowSealedBoxShot() {
}

void BA_ShowSealedBoxShot::Initialize() {
	//現在時間を初期化
	elapsedTimer_ = duration_;

}

void BA_ShowSealedBoxShot::Update() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");

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

	//カメラの座標と回転を線形補完で動かす
	Vector3 cameraTranslate = camera->worldTransform.GetTranslate();
	cameraTranslate = MyMath::Lerp(startCameraTransform_.translate, endCameraTransform_.translate, MyMath::EaseInSine(1.0f - (elapsedTimer_ / duration_)));
	Vector3 cameraRotate = camera->worldTransform.GetRotate();
	cameraRotate = MyMath::Lerp(startCameraTransform_.rotate, endCameraTransform_.rotate, MyMath::EaseInSine(1.0f - (elapsedTimer_ / duration_)));
	//結果をセット
	camera->worldTransform.SetTranslate(cameraTranslate);
	camera->worldTransform.SetRotate(cameraRotate);

	
}

void BA_ShowSealedBoxShot::Finalize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");

	//カメラの座標、回転を最終結果に合わせる
	camera->worldTransform.SetTranslate(endCameraTransform_.translate);
	camera->worldTransform.SetRotate(endCameraTransform_.rotate);

}

void BA_ShowSealedBoxShot::Debug() {
}
