#include "BombAttackLeaf.h"
#include <ImGuiManager.h>

BombAttackLeaf::BombAttackLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

BombAttackLeaf::~BombAttackLeaf() {
}

void BombAttackLeaf::Update() {
}

void BombAttackLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("爆弾攻撃");
	ImGui::End();
#endif // _DEBUG
}

NodeResult BombAttackLeaf::GetNodeResult() const {
	//常に成功を返す
	return NodeResult::Success;
}
