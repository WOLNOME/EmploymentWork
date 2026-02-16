#include "LevelSealedBox.h"
#include <Object3dManager.h>

using namespace Norm;

void LevelSealedBox::Initialize(const std::string& _name) {
	//基底クラスの初期化
	IBaseLevelObject::Initialize(_name);
	//オブジェクトの初期化
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName(_name), "sealedBox");

}

void LevelSealedBox::Update() {
	//基底クラスの更新
	IBaseLevelObject::Update();

}

void LevelSealedBox::DebugWithImGui() {
	//基底クラスのデバッグ
	IBaseLevelObject::DebugWithImGui();

}
