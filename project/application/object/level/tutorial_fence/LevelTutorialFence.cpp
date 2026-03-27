#include "LevelTutorialFence.h"
#include <Object3dManager.h>

using namespace Norm;

void LevelTutorialFence::Initialize(const std::string& _name) {
	//基底クラスの初期化
	IBaseLevelObject::Initialize(_name);
	//オブジェクトの初期化
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName(_name), "tutorial_fence");

}

void LevelTutorialFence::Update() {
	//基底クラスの更新
	IBaseLevelObject::Update();
}

void LevelTutorialFence::DebugWithImGui() {
	//基底クラスのデバッグ
	IBaseLevelObject::DebugWithImGui();
}
