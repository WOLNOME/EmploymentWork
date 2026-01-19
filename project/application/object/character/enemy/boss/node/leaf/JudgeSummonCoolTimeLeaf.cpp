#include "JudgeSummonCoolTimeLeaf.h"
#include <ImGuiManager.h>

JudgeSummonCoolTimeLeaf::JudgeSummonCoolTimeLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

JudgeSummonCoolTimeLeaf::~JudgeSummonCoolTimeLeaf() {
}

void JudgeSummonCoolTimeLeaf::Update() {
}

void JudgeSummonCoolTimeLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("召喚クールタイム判定");
	ImGui::End();
#endif // _DEBUG
}

NodeResult JudgeSummonCoolTimeLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	float summonCoolTimer = mpBlackBoard->GetValue<float>("SummonCoolTimer");

	//もしクールタイム中ならfailを返す
	if (summonCoolTimer > 0.0f) {
		return NodeResult::Fail;
	}

	//クールタイムが終了しているならsuccessを返す
	return NodeResult::Success;
}
