#include "TankEnemyDeadState.h"

#include <MyMath.h>
#include <CombinedParticleManager.h>

//アプリケーション
#include <application/object/character/enemy/tank/base/IBaseTankEnemy.h>
#include <application/object/character/player/Player.h>

using namespace Norm;

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
	//仮死状態にする
	enemy->SetState(BaseCharacter::State::kAsphyxia);
}

void TankEnemyDeadState::Update(IBaseTankEnemy* enemy) {
	//パーティクルが終了したらアイドル状態にする
	if (!particle_->GetIsPlay()) {
		enemy->SetState(BaseCharacter::State::kIdle);
	}
}

void TankEnemyDeadState::Exit(IBaseTankEnemy* enemy) {
}
