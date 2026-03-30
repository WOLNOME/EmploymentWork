#include "LevelBrokenTank.h"
#include <Object3dManager.h>
#include <CombinedParticleManager.h>

using namespace Norm;

void LevelBrokenTank::Initialize(const std::string& _name) {
	//基底クラスの初期化
	IBaseLevelObject::Initialize(_name);
	//オブジェクトの初期化
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName(_name), "brokenTank");
}

void LevelBrokenTank::Update() {
	//基底クラスの更新
	IBaseLevelObject::Update();
}

void LevelBrokenTank::DebugWithImGui() {
	//基底クラスのデバッグ
	IBaseLevelObject::DebugWithImGui();
}

void LevelBrokenTank::InitParticle() {
	//パーティクル
	{
		//オブジェクトデータを走査
		for (auto& [handle, worldTransform] : worldTransforms_) {
			//パーティクルを生成
			std::unique_ptr<CombinedParticle> particle = std::make_unique<CombinedParticle>();
			particle->Initialize(CombinedParticleManager::GetInstance()->GenerateName("smoke"),"BlackSmoke");
			//基準トランスフォームをセット
			TransformEuler baseTransform;
			baseTransform.scale = { 1,1,1 };
			baseTransform.rotate = { 0,0,0 };
			baseTransform.translate = worldTransform->GetTranslate();
			particle->SetBaseTransform(baseTransform);
			//再生
			particle->SetIsPlay(true);
			particle->SetIsRepeat(true);

			particles_.push_back(std::move(particle));
		}
	}
}

