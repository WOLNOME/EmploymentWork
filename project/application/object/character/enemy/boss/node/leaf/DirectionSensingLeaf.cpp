#include "DirectionSensingLeaf.h"
#include <ImGuiManager.h>

//アプリケーション
#include <application/ui/enemy/EnemyUI.h>

using namespace Norm;

DirectionSensingLeaf::DirectionSensingLeaf(int _nodeID, BlackBoard* _blackBoard) : LeafNodeBase(_nodeID, _blackBoard) {
}

DirectionSensingLeaf::~DirectionSensingLeaf() {
}

void DirectionSensingLeaf::Initialize() {
	//基底クラスの初期化
	LeafNodeBase::Initialize();
}

void DirectionSensingLeaf::Update() {

	//ブラックボードから必要な情報を取得
	EnemyUI* enemyUI = mpBlackBoard->GetValue<EnemyUI*>("EnemyUI");
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	float uiHeight = mpBlackBoard->GetValue<float>("UIHeight");
	float uiFront = mpBlackBoard->GetValue<float>("UIFront");

	//発見UIを表示
	enemyUI->GetEnemyReactionUI()->SensingSpawn(bossPos, uiHeight, uiFront);

}

void DirectionSensingLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("方向発見演出");
	ImGui::End();
#endif // _DEBUG
}

NodeResult DirectionSensingLeaf::GetNodeResult() const {
	//successを返す
	return NodeResult::Success;
}
