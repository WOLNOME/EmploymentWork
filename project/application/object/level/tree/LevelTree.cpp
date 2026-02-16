#include "LevelTree.h"
#include <Object3dManager.h>

using namespace Norm;

void LevelTree::Initialize(const std::string& _name) {
	//基底クラスの初期化
	IBaseLevelObject::Initialize(_name);
	//オブジェクトの初期化
	object3d_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName(_name), "tree");

}

void LevelTree::Update() {
	//基底クラスの更新
	IBaseLevelObject::Update();

	//倒木処理
	TreeInvertProcess();
}

void LevelTree::DebugWithImGui() {
	//基底クラスのデバッグ
	IBaseLevelObject::DebugWithImGui();
}

void LevelTree::TreeInvertProcess() {
	//ツリーデータを走査
	for (auto& [handle, treeData] : treeDatas_) {
		//アクションフラグが立っているなら
		if (treeData.isAction) {
			//アクション中ならタイマーを進める
			treeData.invertTimer += kDeltaTime;
			treeData.invertTimer = std::min(treeData.invertTimer, kInvertTime_);

			//新回転
			Vector3 newRotate = worldTransforms_[handle]->GetRotate();

			//Lerpで倒れる方向に回転
			newRotate.x = MyMath::Lerp(0.0f, treeData.invertDirection.z * -(pi / 2.0f), MyMath::EaseInOutSine(treeData.invertTimer / kInvertTime_));
			newRotate.z = MyMath::Lerp(0.0f, treeData.invertDirection.x * (pi / 2.0f), MyMath::EaseInOutSine(treeData.invertTimer / kInvertTime_));

			//新回転をセット
			worldTransforms_[handle]->SetRotate(newRotate);

			//タイマーが経過したらアクションを終了
			if (treeData.invertTimer >= kInvertTime_) {
				treeData.isAction = false;
				treeData.invertTimer = 0.0f;
			}
		}
	}

}