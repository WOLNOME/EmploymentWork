#include "BD_ZoomOutShot.h"
#include <Audio.h>
#include <vector>
#include <random>
#include <BaseCamera.h>
#include <GameCamera.h>
#include <Object3d.h>
#include <CombinedParticle.h>

using namespace Norm;

BD_ZoomOutShot::BD_ZoomOutShot(int _index, Norm::TransformEuler _startCameraTransform, Norm::TransformEuler _endCameraTransform, float _duration, Norm::BlackBoard* _blackBoard) : IShotBase(_index, _startCameraTransform, _endCameraTransform, _duration, _blackBoard) {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/bossDestroySystem");

}

BD_ZoomOutShot::~BD_ZoomOutShot() {
}

void BD_ZoomOutShot::Initialize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");
	std::vector<CombinedParticle*> blackSmokes = blackBoard_->GetValue<std::vector<CombinedParticle*>>("BlackSmoke_Particle");
	Audio* ground_shake = blackBoard_->GetValue<Audio*>("Ground_Shake_SE");

	//カメラの座標、回転を初期状態に合わせる
	{
		camera->worldTransform.SetTranslate(startCameraTransform_.translate);
		camera->worldTransform.SetRotate(startCameraTransform_.rotate);
	}
	//ランダムで黒煙を出す処理
	{
		int particleCount = static_cast<int>(blackSmokes.size());

		Vector3 center = {
			param_["particleBox"]["center"]["x"],
			param_["particleBox"]["center"]["y"],
			param_["particleBox"]["center"]["z"]
		};

		Vector3 boxSize = {
			param_["particleBox"]["size"]["x"],
			param_["particleBox"]["size"]["y"],
			param_["particleBox"]["size"]["z"]
		};

		// ランダムエンジン
		std::random_device rd;
		std::mt19937 mt(rd());

		std::uniform_real_distribution<float> distX(center.x - boxSize.x, center.x + boxSize.x);
		std::uniform_real_distribution<float> distY(center.y - boxSize.y, center.y + boxSize.y);
		std::uniform_real_distribution<float> distZ(center.z - boxSize.z, center.z + boxSize.z);

		for (int i = 0; i < particleCount; i++) {

			Vector3 pos = {
				distX(mt),
				distY(mt),
				distZ(mt)
			};

			blackSmokes[i]->SetBaseTransform({ { 1,1,1 }, { 0,0,0 }, pos });
			blackSmokes[i]->SetIsPlay(true);
		}
	}
	//地響きSE
	{
		ground_shake->Play(true, 0.5f);
	}

	//現在時間を初期化
	elapsedTimer_ = duration_;

}

void BD_ZoomOutShot::Update() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");
	std::vector<CombinedParticle*> mediunExplosions = blackBoard_->GetValue<std::vector<CombinedParticle*>>("MediumExplosion_Particle");
	Audio* explosion_small = blackBoard_->GetValue<Audio*>("Explosion_Small_SE");

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

	//シェイク
	{
		GameCamera* gameCamera = dynamic_cast<GameCamera*>(camera);
		float shakeTime = param_["shakeShortTime"];
		float shakePower = param_["shakeSmallPower"];
		gameCamera->RegistShake(shakeTime, shakePower);
	}

	//中規模爆発をランダムに発生させる処理
	{
		static std::random_device rd;
		static std::mt19937 mt(rd());

		// --- 範囲取得（黒煙と同じ） ---
		Vector3 center = {
			param_["particleBox"]["center"]["x"],
			param_["particleBox"]["center"]["y"],
			param_["particleBox"]["center"]["z"]
		};

		Vector3 boxSize = {
			param_["particleBox"]["size"]["x"],
			param_["particleBox"]["size"]["y"],
			param_["particleBox"]["size"]["z"]
		};

		std::uniform_real_distribution<float> distX(center.x - boxSize.x, center.x + boxSize.x);
		std::uniform_real_distribution<float> distY(center.y - boxSize.y, center.y + boxSize.y);
		std::uniform_real_distribution<float> distZ(center.z - boxSize.z, center.z + boxSize.z);

		// --- 発生タイミングをランダム制御 ---
		// 例：1フレームあたり発生確率 5%
		std::uniform_real_distribution<float> spawnChance(0.0f, 1.0f);

		if (spawnChance(mt) < 0.05f)   // ← 発生確率調整ポイント
		{
			// 再生していないパーティクルを探す
			for (auto* particle : mediunExplosions) {
				if (!particle->GetIsPlay()) {
					Vector3 pos = {
						distX(mt),
						distY(mt),
						distZ(mt)
					};

					particle->SetBaseTransform({ { 1,1,1 }, { 0,0,0 }, pos });
					particle->SetIsPlay(true);

					//小爆発SE
					explosion_small->Play(false, 1.0f);

					break; // 1個だけ発生させる
				}
			}
		}
	}


}

void BD_ZoomOutShot::Finalize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");
	std::vector<CombinedParticle*> blackSmokes = blackBoard_->GetValue<std::vector<CombinedParticle*>>("BlackSmoke_Particle");

	//カメラの座標、回転を最終結果に合わせる
	{
		camera->worldTransform.SetTranslate(endCameraTransform_.translate);
		camera->worldTransform.SetRotate(endCameraTransform_.rotate);
	}

}

void BD_ZoomOutShot::Debug() {
}
