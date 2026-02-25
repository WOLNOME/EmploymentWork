#include "BA_UnlockShot.h"
#include <BaseCamera.h>
#include <WorldTransform.h>
#include <CombinedParticle.h>

using namespace Norm;

BA_UnlockShot::BA_UnlockShot(int _index, Norm::TransformEuler _startCameraTransform, Norm::TransformEuler _endCameraTransform, float _duration, Norm::BlackBoard* _blackBoard) : IShotBase(_index, _startCameraTransform, _endCameraTransform, _duration, _blackBoard) {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/bossAppearSystem");

}

BA_UnlockShot::~BA_UnlockShot() {
}

void BA_UnlockShot::Initialize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");
	
	//カメラの座標、回転を初期状態に合わせる
	{
		camera->worldTransform.SetTranslate(startCameraTransform_.translate);
		camera->worldTransform.SetRotate(startCameraTransform_.rotate);
	}

	//現在時間を初期化
	elapsedTimer_ = duration_;

}

void BA_UnlockShot::Update() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");
	WorldTransform* wtKey1 = blackBoard_->GetValue<WorldTransform*>("Key1_WorldTransform");
	WorldTransform* wtKey2 = blackBoard_->GetValue<WorldTransform*>("Key2_WorldTransform");

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

	//キーを動かす
	{
		//キー1
		Vector3 startPos1 = {
			param_["key1"]["startTransform"]["translate"]["x"],
			param_["key1"]["startTransform"]["translate"]["y"],
			param_["key1"]["startTransform"]["translate"]["z"]
		};
		Vector3 endPos1 = {
			param_["key1"]["endTransform"]["translate"]["x"],
			param_["key1"]["endTransform"]["translate"]["y"],
			param_["key1"]["endTransform"]["translate"]["z"]
		};
		Vector3 easePos1 = MyMath::Lerp(startPos1, endPos1, MyMath::EaseInSine(1.0f - (elapsedTimer_ / duration_)));
		wtKey1->SetTranslate(easePos1);

		//キー2
		Vector3 startPos2{
			param_["key2"]["startTransform"]["translate"]["x"],
			param_["key2"]["startTransform"]["translate"]["y"],
			param_["key2"]["startTransform"]["translate"]["z"]
		};
		Vector3 endPos2 = {
			param_["key2"]["endTransform"]["translate"]["x"],
			param_["key2"]["endTransform"]["translate"]["y"],
			param_["key2"]["endTransform"]["translate"]["z"]
		};
		Vector3 easePos2 = MyMath::Lerp(startPos2, endPos2, MyMath::EaseOutSine(1.0f - (elapsedTimer_ / duration_)));
		wtKey2->SetTranslate(easePos2);
	}

}

void BA_UnlockShot::Finalize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");
	WorldTransform* wtKey1 = blackBoard_->GetValue<WorldTransform*>("Key1_WorldTransform");
	WorldTransform* wtKey2 = blackBoard_->GetValue<WorldTransform*>("Key2_WorldTransform");
	CombinedParticle* flush1 = blackBoard_->GetValue<CombinedParticle*>("Flush1_Particle");
	CombinedParticle* flush2 = blackBoard_->GetValue<CombinedParticle*>("Flush2_Particle");

	//カメラの座標、回転を最終結果に合わせる
	{
		camera->worldTransform.SetTranslate(endCameraTransform_.translate);
		camera->worldTransform.SetRotate(endCameraTransform_.rotate);
	}

	//キーの位置を最終結果に合わせる
	{
		//キー1
		Vector3 endPos1 = {
			param_["key1"]["endTransform"]["translate"]["x"],
			param_["key1"]["endTransform"]["translate"]["y"],
			param_["key1"]["endTransform"]["translate"]["z"]
		};
		wtKey1->SetTranslate(endPos1);
		//フラッシュ1
		flush1->SetIsPlay(true);
		flush1->SetBaseTransform(TransformEuler({ 1,1,1 }, { 0,0,0 }, endPos1));

		//キー2
		Vector3 endPos2 = {
			param_["key2"]["endTransform"]["translate"]["x"],
			param_["key2"]["endTransform"]["translate"]["y"],
			param_["key2"]["endTransform"]["translate"]["z"]
		};
		wtKey2->SetTranslate(endPos2);
		//フラッシュ2
		flush2->SetIsPlay(true);
		flush2->SetBaseTransform(TransformEuler({ 1,1,1 }, { 0,0,0 }, endPos2));
	}

}

void BA_UnlockShot::Debug() {
}
