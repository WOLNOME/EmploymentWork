#include "PatrolLeaf.h"
#include <ImGuiManager.h>

using namespace Norm;

PatrolLeaf::PatrolLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

PatrolLeaf::~PatrolLeaf() {
}

void PatrolLeaf::Update() {
	//今はとくに何もしない
}

void PatrolLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("パトロール");
	ImGui::End();
#endif // _DEBUG
}

NodeResult PatrolLeaf::GetNodeResult() const {
	//常にSuccessを返す
	return NodeResult::Success;
}
