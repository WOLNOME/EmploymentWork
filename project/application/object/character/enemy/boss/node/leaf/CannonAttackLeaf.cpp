#include "CannonAttackLeaf.h"
#include <ImGuiManager.h>

CannonAttackLeaf::CannonAttackLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

CannonAttackLeaf::~CannonAttackLeaf() {
}

void CannonAttackLeaf::Update() {
}

void CannonAttackLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("キャノン攻撃");
	ImGui::End();
#endif // _DEBUG
}

NodeResult CannonAttackLeaf::GetNodeResult() const {
	//常に成功を返す
	return NodeResult::Success;
}
