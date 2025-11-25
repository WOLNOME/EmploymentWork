#include "DeathDirection.h"
#include <cassert>
#include <ParticleManager.h>
#include <random>

void DeathDirection::Initialize() {
	//パーティクルの初期化
	particle_ = std::make_unique<Particle>();
	particle_->Initialize(ParticleManager::GetInstance()->GenerateName("playerDead"), "enemy_explosion");
	particle_->emitter_.isPlay = false;
	particle_->emitter_.transform.scale = { 0.1f,0.1f,0.1f };
	particle_->emitter_.effectStyle = Particle::EffectStyle::Loop;
	particle_->emitter_.isGravity = true;
	particle_->emitter_.gravity = -10.0f;
	particle_->emitter_.isBound = true;
	particle_->emitter_.floorHeight = 0.0f;
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
	particle_->emitter_.isPlay = true;

	//エミッターの座標を常にカメラへ
	particle_->emitter_.transform.translate = camera_->worldTransform.worldTranslate;

	//エミッターのサイズを少し広げる
	particle_->emitter_.transform.scale = { 3.0f,3.0f,3.0f };

}
