#include "JudgeBarrierCoolTimeLeaf.h"
#include <ImGuiManager.h>

JudgeBarrierCoolTimeLeaf::JudgeBarrierCoolTimeLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

JudgeBarrierCoolTimeLeaf::~JudgeBarrierCoolTimeLeaf() {
}

void JudgeBarrierCoolTimeLeaf::Update() {
}

void JudgeBarrierCoolTimeLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("バリアクールタイム判定");
	ImGui::End();
#endif // _DEBUG
}

NodeResult JudgeBarrierCoolTimeLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	float barrierCoolTimer = mpBlackBoard->GetValue<float>("BarrierCoolTimer");

	//もしクールタイム中ならfailを返す
	if (barrierCoolTimer > 0.0f) {
		return NodeResult::Fail;
	}

	//クールタイムが終了しているならsuccessを返す
	return NodeResult::Success;
}
