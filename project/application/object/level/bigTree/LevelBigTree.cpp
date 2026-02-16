#include "LevelBigTree.h"
#include <Object3dManager.h>

using namespace Norm;

void LevelBigTree::Initialize(const std::string& _name) {
	//基底クラスの初期化
	IBaseLevelObject::Initialize(_name);
	//オブジェクトの初期化
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName(_name), "bigTree");

}

void LevelBigTree::Update() {
	//基底クラスの更新
	IBaseLevelObject::Update();

}

void LevelBigTree::DebugWithImGui() {
	//基底クラスのデバッグ
	IBaseLevelObject::DebugWithImGui();

}
