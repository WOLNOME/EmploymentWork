#include "DirectionMissingLeaf.h"
#include <ImGuiManager.h>

//アプリケーション
#include <application/ui/enemy/EnemyUI.h>

using namespace Norm;

DirectionMissingLeaf::DirectionMissingLeaf(int _nodeID, BlackBoard* _blackBoard) : LeafNodeBase(_nodeID, _blackBoard) {
}

DirectionMissingLeaf::~DirectionMissingLeaf() {
}

void DirectionMissingLeaf::Initialize() {
	//基底クラスの初期化
	LeafNodeBase::Initialize();
}

void DirectionMissingLeaf::Update() {
	//ブラックボードから必要な情報を取得
	EnemyUI* enemyUI = mpBlackBoard->GetValue<EnemyUI*>("EnemyUI");
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	float uiHeight = mpBlackBoard->GetValue<float>("UIHeight");
	float uiFront = mpBlackBoard->GetValue<float>("UIFront");

	//見失うUIを表示
	enemyUI->GetEnemyReactionUI()->MissingSpawn(bossPos, uiHeight, uiFront);

}

void DirectionMissingLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("方向見失い演出");
	ImGui::End();
#endif // _DEBUG
}

NodeResult DirectionMissingLeaf::GetNodeResult() const {
	//successを返す
	return NodeResult::Success;
}
