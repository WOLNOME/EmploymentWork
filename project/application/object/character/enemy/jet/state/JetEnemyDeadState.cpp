#include "JetEnemyDeadState.h"

#include <MyMath.h>
#include <CombinedParticleManager.h>
//アプリケーション
#include <application/object/character/enemy/jet/base/IBaseJetEnemy.h>
#include <application/object/character/player/Player.h>

JetEnemyDeadState::JetEnemyDeadState() {
	//パーティクルの生成・初期化
	particle_ = std::make_unique<CombinedParticle>();
	particle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("JetEnemyDead"), "Enemy_Explosion");
}

void JetEnemyDeadState::Enter(IBaseJetEnemy* enemy) {
	//再生
	particle_->SetIsPlay(true);
	//基準トランスフォームのセット
	TransformEuler transform = particle_->GetBaseTransform();
	transform.translate = enemy->GetWorldPosition();
	particle_->SetBaseTransform(transform);
	//仮死状態にする
	enemy->SetState(BaseCharacter::State::kAsphyxia);
}

void JetEnemyDeadState::Update(IBaseJetEnemy* enemy) {
	//パーティクルが終了したらアイドル状態にする
	if (!particle_->GetIsPlay()) {
		enemy->SetState(BaseCharacter::State::kIdle);
	}
}

void JetEnemyDeadState::Exit(IBaseJetEnemy* enemy) {
}
