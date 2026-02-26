#include "BD_BigExplosion1Shot.h"
#include <BaseCamera.h>
#include <GameCamera.h>
#include <Object3d.h>
#include <CombinedParticle.h>

using namespace Norm;

BD_BigExplosion1Shot::BD_BigExplosion1Shot(int _index, Norm::TransformEuler _startCameraTransform, Norm::TransformEuler _endCameraTransform, float _duration, Norm::BlackBoard* _blackBoard) : IShotBase(_index, _startCameraTransform, _endCameraTransform, _duration, _blackBoard) {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/bossDestroySystem");

}

BD_BigExplosion1Shot::~BD_BigExplosion1Shot() {

}

void BD_BigExplosion1Shot::Initialize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");
	std::vector<CombinedParticle*> hugeExplosions = blackBoard_->GetValue<std::vector<CombinedParticle*>>("HugeExplosion_Particle");

	//カメラの座標、回転を初期状態に合わせる
	{
		camera->worldTransform.SetTranslate(startCameraTransform_.translate);
		camera->worldTransform.SetRotate(startCameraTransform_.rotate);
	}

	//シェイク
	{
		GameCamera* gameCamera = dynamic_cast<GameCamera*>(camera);
		float shakeTime = param_["shakeLongTime"];
		float shakePower = param_["shakeHugePower"];
		gameCamera->RegistShake(shakeTime, shakePower);
	}

	//巨大爆発処理
	{
		Vector3 center = {
			param_["particleBox"]["center"]["x"],
			param_["particleBox"]["center"]["y"],
			param_["particleBox"]["center"]["z"]
		};

		for (auto* particle : hugeExplosions) {
			if (!particle->GetIsPlay()) {
				particle->SetBaseTransform({ { 1,1,1 }, { 0,0,0 }, center });
				particle->SetIsPlay(true);
				break; // 1つだけ出す
			}
		}
	}

	//現在時間を初期化
	elapsedTimer_ = duration_;

}

void BD_BigExplosion1Shot::Update() {
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

void BD_BigExplosion1Shot::Finalize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");

	//カメラの座標、回転を最終結果に合わせる
	{
		camera->worldTransform.SetTranslate(endCameraTransform_.translate);
		camera->worldTransform.SetRotate(endCameraTransform_.rotate);
	}

}

void BD_BigExplosion1Shot::Debug() {
}
