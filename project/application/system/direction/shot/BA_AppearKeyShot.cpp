#include "BA_AppearKeyShot.h"
#include <BaseCamera.h>
#include <Object3d.h>
#include <WorldTransform.h>
#include <CombinedParticle.h>

using namespace Norm;

BA_AppearKeyShot::BA_AppearKeyShot(int _index, Norm::TransformEuler _startCameraTransform, Norm::TransformEuler _endCameraTransform, float _duration, Norm::BlackBoard* _blackBoard) : IShotBase(_index, _startCameraTransform, _endCameraTransform, _duration, _blackBoard) {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/bossAppearSystem");

}

BA_AppearKeyShot::~BA_AppearKeyShot() {
}

void BA_AppearKeyShot::Initialize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");
	Object3d* key1 = blackBoard_->GetValue<Object3d*>("Key1_Object");
	Object3d* key2 = blackBoard_->GetValue<Object3d*>("Key2_Object");
	WorldTransform* wtKey1 = blackBoard_->GetValue<WorldTransform*>("Key1_WorldTransform");
	WorldTransform* wtKey2 = blackBoard_->GetValue<WorldTransform*>("Key2_WorldTransform");
	CombinedParticle* flush1 = blackBoard_->GetValue<CombinedParticle*>("Flush1_Particle");
	CombinedParticle* flush2 = blackBoard_->GetValue<CombinedParticle*>("Flush2_Particle");

	//カメラの座標、回転を初期状態に合わせる
	{
		camera->worldTransform.SetTranslate(startCameraTransform_.translate);
		camera->worldTransform.SetRotate(startCameraTransform_.rotate);
	}

	//キーを出現
	{
		//キー1を出現
		key1->SetIsDisplay(true);

		//トランスフォームをセット
		Vector3 pos1 = {
			param_["key1"]["startTransform"]["translate"]["x"],
			param_["key1"]["startTransform"]["translate"]["y"],
			param_["key1"]["startTransform"]["translate"]["z"]
		};
		Vector3 rot1 = {
			param_["key1"]["startTransform"]["rotate"]["x"],
			param_["key1"]["startTransform"]["rotate"]["y"],
			param_["key1"]["startTransform"]["rotate"]["z"]
		};
		Vector3 sca1 = {
			param_["key1"]["startTransform"]["scale"]["x"],
			param_["key1"]["startTransform"]["scale"]["y"],
			param_["key1"]["startTransform"]["scale"]["z"]
		};
		wtKey1->SetTranslate(pos1);
		wtKey1->SetRotate(rot1);
		wtKey1->SetScale(sca1);

		//フラッシュ1を出現
		flush1->SetIsPlay(true);
		flush1->SetBaseTransform(TransformEuler({ 1,1,1 }, { 0,0,0 }, pos1));

		//キー2を出現
		key2->SetIsDisplay(true);

		//トランスフォームをセット
		Vector3 pos2 = {
			param_["key2"]["startTransform"]["translate"]["x"],
			param_["key2"]["startTransform"]["translate"]["y"],
			param_["key2"]["startTransform"]["translate"]["z"]
		};
		Vector3 rot2 = {
			param_["key2"]["startTransform"]["rotate"]["x"],
			param_["key2"]["startTransform"]["rotate"]["y"],
			param_["key2"]["startTransform"]["rotate"]["z"]
		};
		Vector3 sca2 = {
			param_["key2"]["startTransform"]["scale"]["x"],
			param_["key2"]["startTransform"]["scale"]["y"],
			param_["key2"]["startTransform"]["scale"]["z"]
		};
		wtKey2->SetTranslate(pos2);
		wtKey2->SetRotate(rot2);
		wtKey2->SetScale(sca2);

		//フラッシュ2を出現
		flush2->SetIsPlay(true);
		flush2->SetBaseTransform(TransformEuler({ 1,1,1 }, { 0,0,0 }, pos2));

	}

	//現在時間を初期化
	elapsedTimer_ = duration_;

}

void BA_AppearKeyShot::Update() {
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

void BA_AppearKeyShot::Finalize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");

	//カメラの座標、回転を最終結果に合わせる
	camera->worldTransform.SetTranslate(endCameraTransform_.translate);
	camera->worldTransform.SetRotate(endCameraTransform_.rotate);
}

void BA_AppearKeyShot::Debug() {
}
