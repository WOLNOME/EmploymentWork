#include "TankEnemyDeadState.h"

#include <MyMath.h>
#include <CombinedParticleManager.h>

//アプリケーション
#include <application/object/character/enemy/tank/base/IBaseTankEnemy.h>
#include <application/object/character/player/Player.h>

TankEnemyDeadState::TankEnemyDeadState() {
	//パーティクルの生成・初期化
	particle_ = std::make_unique<CombinedParticle>();
	particle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("TankEnemyDead"), "Enemy_Explosion");
}

void TankEnemyDeadState::Enter(IBaseTankEnemy* enemy) {
	//再生
	particle_->SetIsPlay(true);
	//基準トランスフォームのセット
	TransformEuler transform = particle_->GetBaseTransform();
	transform.translate = enemy->GetWorldPosition();
	particle_->SetBaseTransform(transform);
	//死亡タイマーをセット(設計上上書きされることはない)
	enemy->SetDeadTimer(particle_->GetDuration());
}

void TankEnemyDeadState::Update(IBaseTankEnemy* enemy) {
}

void TankEnemyDeadState::Exit(IBaseTankEnemy* enemy) {
}
