#include "JetEnemyDeadState.h"

#include <MyMath.h>
#include <ParticleManager.h>

//アプリケーション
#include <application/object/character/enemy/jet/base/IBaseJetEnemy.h>
#include <application/object/character/player/Player.h>

JetEnemyDeadState::JetEnemyDeadState() {
	//パーティクルの生成・初期化
	particle_ = std::make_unique<Particle>();
	particle_->Initialize(ParticleManager::GetInstance()->GenerateName("EnemyDead"), "enemy_explosion");
	particle_->emitter_.isPlay = false;
	particle_->emitter_.transform.scale = { 0.1f,0.1f,0.1f };
}

void JetEnemyDeadState::Enter(IBaseJetEnemy* enemy) {
}

void JetEnemyDeadState::Update(IBaseJetEnemy* enemy) {
	particleOnTimer_ +=kDeltaTime;
	//死亡パーティクルをオン
	particle_->emitter_.transform.translate = enemy->GetWorldTransform().translate;
	particle_->emitter_.isPlay = true;
	//時間を超えたらオフにする
	if (particleOnTimer_ > particleOnTime_) {
		particle_->emitter_.isPlay = false;
	}

	//死亡タイマーをセット(設計上上書きされることはない)
	float particleLifeTime = particle_->GetParam()["LifeTime"]["Max"];
	enemy->SetDeadTimer(float(particleOnTime_ + particleLifeTime));
}

void JetEnemyDeadState::Exit(IBaseJetEnemy* enemy) {
}
