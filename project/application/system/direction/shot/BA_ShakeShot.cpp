#include "BA_ShakeShot.h"
#include <Audio.h>
#include <BaseCamera.h>
#include <GameCamera.h>

using namespace Norm;

BA_ShakeShot::BA_ShakeShot(int _index, Norm::TransformEuler _startCameraTransform, Norm::TransformEuler _endCameraTransform, float _duration, Norm::BlackBoard* _blackBoard) : IShotBase(_index, _startCameraTransform, _endCameraTransform, _duration, _blackBoard) {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/bossAppearSystem");
}

BA_ShakeShot::~BA_ShakeShot() {
}

void BA_ShakeShot::Initialize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");
	Audio* ground_shake = blackBoard_->GetValue<Audio*>("Ground_Shake_SE");

	//カメラの座標、回転を初期状態に合わせる
	{
		camera->worldTransform.SetTranslate(startCameraTransform_.translate);
		camera->worldTransform.SetRotate(startCameraTransform_.rotate);
	}

	//地響きSE
	{
		ground_shake->Play(true, 0.5f);
	}

	//現在時間を初期化
	elapsedTimer_ = duration_;

}

void BA_ShakeShot::Update() {
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

	//シェイク処理
	GameCamera* gameCamera = dynamic_cast<GameCamera*>(camera);
	float shakeTime = param_["shakeTime"];
	float shakePower = param_["shakePower"];
	gameCamera->RegistShake(shakeTime, shakePower);
}

void BA_ShakeShot::Finalize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");

	//カメラの座標、回転を最終結果に合わせる
	camera->worldTransform.SetTranslate(endCameraTransform_.translate);
	camera->worldTransform.SetRotate(endCameraTransform_.rotate);
}

void BA_ShakeShot::Debug() {
}
