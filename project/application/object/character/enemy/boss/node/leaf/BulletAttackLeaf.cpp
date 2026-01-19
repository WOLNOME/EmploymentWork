#include "BulletAttackLeaf.h"
#include <ImGuiManager.h>

BulletAttackLeaf::BulletAttackLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

BulletAttackLeaf::~BulletAttackLeaf() {
}

void BulletAttackLeaf::Update() {
}

void BulletAttackLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("弾丸攻撃");
	ImGui::End();
#endif // _DEBUG
}

NodeResult BulletAttackLeaf::GetNodeResult() const {
	//常に成功を返す
	return NodeResult::Success;
}
