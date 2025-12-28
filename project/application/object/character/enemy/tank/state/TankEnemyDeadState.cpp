#include "TankEnemyDeadState.h"

#include <MyMath.h>
#include <ParticleManager.h>

//アプリケーション
#include <application/object/character/enemy/tank/base/IBaseTankEnemy.h>
#include <application/object/character/player/Player.h>

TankEnemyDeadState::TankEnemyDeadState() {
	//パーティクルの生成・初期化
	particle_ = std::make_unique<Particle>();
	particle_->Initialize(ParticleManager::GetInstance()->GenerateName("EnemyDead"), "enemy_explosion");
	particle_->emitter_.isPlay = false;
	//particle_->emitter_.transform.scale = { 0.1f,0.1f,0.1f };
}

void TankEnemyDeadState::Enter(IBaseTankEnemy* enemy) {
}

void TankEnemyDeadState::Update(IBaseTankEnemy* enemy) {
	particleOnTimer_ += kDeltaTime;
	//死亡パーティクルをオン
	//particle_->emitter_.transform.translate = enemy->GetWorldPosition();
	particle_->emitter_.isPlay = true;
	//時間を超えたらオフにする
	if (particleOnTimer_ > particleOnTime_) {
		particle_->emitter_.isPlay = false;
	}

	//死亡タイマーをセット(設計上上書きされることはない)
	float particleLifeTime = particle_->GetParam()["LifeTime"]["Max"];
	enemy->SetDeadTimer(float(particleOnTime_ + particleLifeTime));
}

void TankEnemyDeadState::Exit(IBaseTankEnemy* enemy) {
}
