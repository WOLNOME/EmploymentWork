#include "BA_ZoomOutShot.h"
#include <BaseCamera.h>
#include <GameCamera.h>
#include <WorldTransform.h>
#include <CombinedParticle.h>

using namespace Norm;

BA_ZoomOutShot::BA_ZoomOutShot(int _index, Norm::TransformEuler _startCameraTransform, Norm::TransformEuler _endCameraTransform, float _duration, Norm::BlackBoard* _blackBoard) : IShotBase(_index, _startCameraTransform, _endCameraTransform, _duration, _blackBoard) {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/bossAppearSystem");

}

BA_ZoomOutShot::~BA_ZoomOutShot() {
}

void BA_ZoomOutShot::Initialize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");
	CombinedParticle* verge = blackBoard_->GetValue<CombinedParticle*>("Verge_Particle");

	//カメラの座標、回転を初期状態に合わせる
	{
		camera->worldTransform.SetTranslate(startCameraTransform_.translate);
		camera->worldTransform.SetRotate(startCameraTransform_.rotate);
	}

	//封印解除中パーティクルを再生
	{
		//座標を合わせる
		Vector3 pos = { 0.0f,35.0f,0.0f };
		verge->SetBaseTransform(TransformEuler({ 1,1,1 }, { 0,0,0 }, pos));
		//再生
		verge->SetIsPlay(true);
	}


	//現在時間を初期化
	elapsedTimer_ = duration_;

}

void BA_ZoomOutShot::Update() {
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
	{
		GameCamera* gameCamera = dynamic_cast<GameCamera*>(camera);
		float shakeTime = param_["shakeTime"];
		float shakePower = param_["shakePower"];
		gameCamera->RegistShake(shakeTime, shakePower);
	}

	//カメラの座標と回転を線形補完で動かす
	{
		Vector3 cameraTranslate = camera->worldTransform.GetTranslate();
		cameraTranslate = MyMath::Lerp(startCameraTransform_.translate, endCameraTransform_.translate, MyMath::EaseInSine(1.0f - (elapsedTimer_ / duration_)));
		Vector3 cameraRotate = camera->worldTransform.GetRotate();
		cameraRotate = MyMath::Lerp(startCameraTransform_.rotate, endCameraTransform_.rotate, MyMath::EaseInSine(1.0f - (elapsedTimer_ / duration_)));
		//結果をセット
		camera->worldTransform.SetTranslate(cameraTranslate);
		camera->worldTransform.SetRotate(cameraRotate);
	}

}

void BA_ZoomOutShot::Finalize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");
	CombinedParticle* verge = blackBoard_->GetValue<CombinedParticle*>("Verge_Particle");

	//カメラの座標、回転を最終結果に合わせる
	{
		camera->worldTransform.SetTranslate(endCameraTransform_.translate);
		camera->worldTransform.SetRotate(endCameraTransform_.rotate);
	}

	//封印解除中パーティクルを停止
	{
		verge->SetIsPlay(false);
	}

}

void BA_ZoomOutShot::Debug() {
}
