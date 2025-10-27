#include "DeathDirection.h"
#include <cassert>
#include <ParticleManager.h>

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
	CameraContorol();

}

void DeathDirection::DebugWithImGui() {
#ifdef _DEBUG

#endif // _DEBUG
}

void DeathDirection::CameraContorol() {
	//演出フラグがオンでないとreturn
	if (!isDirection_) {
		return;
	}


}
