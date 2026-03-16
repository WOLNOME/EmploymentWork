#include "BA_AppearBossShot.h"
#include <Audio.h>
#include <BaseCamera.h>
#include <Object3d.h>
#include <WorldTransform.h>
#include <CombinedParticle.h>

using namespace Norm;

BA_AppearBossShot::BA_AppearBossShot(int _index, Norm::TransformEuler _startCameraTransform, Norm::TransformEuler _endCameraTransform, float _duration, Norm::BlackBoard* _blackBoard) : IShotBase(_index, _startCameraTransform, _endCameraTransform, _duration, _blackBoard) {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/bossAppearSystem");

}

BA_AppearBossShot::~BA_AppearBossShot() {
}

void BA_AppearBossShot::Initialize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");
	Object3d* boss = blackBoard_->GetValue<Object3d*>("Boss_Object");
	WorldTransform* wtBoss = blackBoard_->GetValue<WorldTransform*>("Boss_WorldTransform");
	CombinedParticle* appear = blackBoard_->GetValue<CombinedParticle*>("Appear_Particle");
	Audio* boss_appear = blackBoard_->GetValue<Audio*>("Boss_Appear_SE");

	//カメラの座標、回転を初期状態に合わせる
	{
		camera->worldTransform.SetTranslate(startCameraTransform_.translate);
		camera->worldTransform.SetRotate(startCameraTransform_.rotate);
	}

	//ボスを出現させる
	{
		//位置と回転を決める
		Vector3 pos = { 0.0f,16.0f,0.0f };
		Vector3 rotate = { 0.0f,3.14f,0.0f };
		wtBoss->SetTranslate(pos);
		wtBoss->SetRotate(rotate);
		//表示
		boss->SetIsDisplay(true);
	}

	//ボス出現パーティクルを再生
	{
		//座標を合わせる
		Vector3 pos = { 0.0f,35.0f,0.0f };
		appear->SetBaseTransform(TransformEuler({ 1,1,1 }, { 0,0,0 }, pos));
		//再生
		appear->SetIsPlay(true);
	}

	//ボス出現SE
	{
		boss_appear->Play(false, 1.0f);
	}

	//現在時間を初期化
	elapsedTimer_ = duration_;

}

void BA_AppearBossShot::Update() {
	//ブラックボードから必要な情報を取得
	Object3d* sealedBox = blackBoard_->GetValue<Object3d*>("SealedBox_Object");
	Object3d* key1 = blackBoard_->GetValue<Object3d*>("Key1_Object");
	Object3d* key2 = blackBoard_->GetValue<Object3d*>("Key2_Object");

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

	//封印ボックスを透明化していく処理
	{
		//目標時間
		float targetTime = duration_ * 0.1f;

		//もし目標時間を超えていたら
		if (targetTime < duration_ - elapsedTimer_) {
			//封印ボックスを非表示にする
			sealedBox->SetIsDisplay(false);
			key1->SetIsDisplay(false);
			key2->SetIsDisplay(false);
		}
		//超えていなかったら透明化処理
		else {
			float alpha = 0.0f;
			alpha = MyMath::Lerp(1.0f, 0.0f, (duration_ - elapsedTimer_) / targetTime);
			sealedBox->SetColor({ 1,1,1,alpha });
			key1->SetColor({ 1,1,1,alpha });
			key2->SetColor({ 1,1,1,alpha });
		}

	}

}

void BA_AppearBossShot::Finalize() {
	//ブラックボードから必要な情報を取得
	BaseCamera* camera = blackBoard_->GetValue<BaseCamera*>("Camera");

	//カメラの座標、回転を最終結果に合わせる
	{
		camera->worldTransform.SetTranslate(endCameraTransform_.translate);
		camera->worldTransform.SetRotate(endCameraTransform_.rotate);
	}
}

void BA_AppearBossShot::Debug() {
}
