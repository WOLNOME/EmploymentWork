#include "DeathDirection.h"
#include <cassert>
#include <CombinedParticleManager.h>
#include <random>

void DeathDirection::Initialize() {
	//パーティクルの初期化
	particle_ = std::make_unique<CombinedParticle>();
	particle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("playerDead"), "Enemy_Explosion");
}

void DeathDirection::Update() {
	//カメラチェック
	assert(camera_ != nullptr && "カメラに値が入っていません。");

	//カメラの操作
	CameraControl();

	//パーティクル演出
	ParticeDir();

}

void DeathDirection::DebugWithImGui() {
#ifdef _DEBUG

#endif // _DEBUG
}

void DeathDirection::CameraControl() {
	//演出フラグがオンでないとreturn
	if (!isDirection_) {
		return;
	}

	//タイマーが0の時に乱数生成する
	if (timer_ == 0.0f) {
		//乱数生成
		std::random_device rd;
		std::mt19937 gen(rd());
		//速度を決定
		{
			std::uniform_real_distribution<float> distXZ(-30.0f, 30.0f);
			std::uniform_real_distribution<float> distY(20.0f, 30.0f);
			velocity_ = { distXZ(gen),distY(gen),distXZ(gen) };
		}
		//回転速度を決定
		{
			std::uniform_real_distribution<float> dist(-3.0f, 3.0f);
			rotateVelocity_.z = dist(gen);
		}

		//カメラのシェイクを開始する
		camera_->RegistShake(kTime_, 0.5f);
	}

	//タイマーを進める
	timer_ += kDeltaTime;
	//時間になったら
	if (timer_ >= kTime_) {
		//演出の終了フラグを立てる
		isDirFinished_ = true;
	}

	//カメラの移動処理
	{
		//重力の加算
		velocity_.y += -kGravity_ * kDeltaTime;
		//速度の加算
		camera_->worldTransform.translate += velocity_ * kDeltaTime;

		//床との当たり判定
		if (camera_->worldTransform.translate.y <= kFloorHeight_) {
			//速度を反転(上向きになるように)
			if (velocity_.y < 0.0f) {
				//速度反転
				velocity_.y = std::fabs(velocity_.y) * kRestitution_;
				//XZ方向の速度を減衰
				velocity_.x *= kRestitution_;
				velocity_.z *= kRestitution_;

				//回転速度の減衰
				rotateVelocity_ *= kRotateDecay_;
			}
		}
	}

	//カメラの回転処理
	{
		//回転加算
		camera_->worldTransform.rotate += rotateVelocity_ * kDeltaTime;
	}

}

void DeathDirection::ParticeDir() {
	//演出フラグがオンでないとreturn
	if (!isDirection_) {
		return;
	}

	//パーティクルをオン
	particle_->SetIsPlay(true);

	//エミッターの座標を常にカメラへ
	TransformEuler transform = particle_->GetBaseTransform();
	transform.translate = camera_->worldTransform.worldTranslate;
	particle_->SetBaseTransform(transform);

}
