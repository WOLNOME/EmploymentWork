#include "GamePlaySystem.h"
#include <CombinedParticleManager.h>
#include <cassert>
#include <MyMath.h>

#include <application/object/character/player/Player.h>

using namespace Norm;

void GamePlaySystem::Initialize() {
	//灰パーティクルの初期化
	{
		ashParticle_ = std::make_unique<CombinedParticle>();
		ashParticle_->Initialize(CombinedParticleManager::GetInstance()->GenerateName("AshParticle"), "Ash");
		ashParticle_->SetIsRepeat(true);
		ashParticle_->SetIsPlay(true);
	}
}

void GamePlaySystem::Update() {
	assert(player_ && "Playerがセットされていません");

	//灰パーティクルの更新
	AshUpdate();
}

void GamePlaySystem::DebugWithImGui() {
}

void GamePlaySystem::AshUpdate() {
	//プレイヤーの位置を基準にパーティクルを移動させる
	TransformEuler baseTransform = {
		.scale = {1,1,1},
		.rotate = {0,0,0},
		.translate = player_->GetWorldTransform().GetTranslate()
	};
	baseTransform.translate.y = 0.0f;

	ashParticle_->SetBaseTransform(baseTransform);
}



