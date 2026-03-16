#include "BD_BigExplosion3Shot.h"
#include <Audio.h>
#include <BaseCamera.h>
#include <GameCamera.h>
#include <Object3d.h>
#include <CombinedParticle.h>

using namespace Norm;

BD_BigExplosion3Shot::BD_BigExplosion3Shot(int _index, Norm::TransformEuler _startCameraTransform, Norm::TransformEuler _endCameraTransform, float _duration, Norm::BlackBoard* _blackBoard) : IShotBase(_index, _startCameraTransform, _endCameraTransform, _duration, _blackBoard) {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/bossDestroySystem");

}

BD_BigExplosion3Shot::~BD_BigExplosion3Shot() {
}

void BD_BigExplosion3Shot::Initialize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");
	std::vector<CombinedParticle*> hugeExplosions = blackBoard_->GetValue<std::vector<CombinedParticle*>>("HugeExplosion_Particle");
	Audio* explosion_large = blackBoard_->GetValue<Audio*>("Explosion_Large_SE");

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

	//大爆発SE
	{
		explosion_large->Play(false, 1.0f);
	}

	//現在時間を初期化
	elapsedTimer_ = duration_;

}

void BD_BigExplosion3Shot::Update() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");
	Object3d* boss = blackBoard_->GetValue<Object3d*>("Boss_Object");
	std::vector<CombinedParticle*> blackSmokes = blackBoard_->GetValue<std::vector<CombinedParticle*>>("BlackSmoke_Particle");

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

	//ボスの透明化処理
	{
		// 経過時間（0 → duration）
		float elapsed = duration_ - elapsedTimer_;

		const float fadeTime = 0.5f;

		float alpha = 1.0f;

		if (elapsed < fadeTime) {
			//α値を線形補完で求める
			alpha = MyMath::Lerp(1.0f, 0.0f, 1.0f - (elapsed / fadeTime));
		}
		else {
			alpha = 0.0f;

			// 完全透明になったら黒煙を停止
			for (auto* smoke : blackSmokes) {
				if (smoke->GetIsPlay()) {
					smoke->SetIsPlay(false);
				}
			}
		}

		boss->SetColor({ 1.0f, 1.0f, 1.0f, alpha });
	}


}

void BD_BigExplosion3Shot::Finalize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");

	//カメラの座標、回転を最終結果に合わせる
	{
		camera->worldTransform.SetTranslate(endCameraTransform_.translate);
		camera->worldTransform.SetRotate(endCameraTransform_.rotate);
	}

}

void BD_BigExplosion3Shot::Debug() {
}
