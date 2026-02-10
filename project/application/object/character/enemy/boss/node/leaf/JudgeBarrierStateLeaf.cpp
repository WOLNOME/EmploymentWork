#include "JudgeBarrierStateLeaf.h"
#include <ImGuiManager.h>

using namespace Norm;

JudgeBarrierStateLeaf::JudgeBarrierStateLeaf(int _nodeID, BlackBoard* _blackBoard) : LeafNodeBase(_nodeID, _blackBoard) {
}

JudgeBarrierStateLeaf::~JudgeBarrierStateLeaf() {
}

void JudgeBarrierStateLeaf::Update() {
}

void JudgeBarrierStateLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("バリア状態の比較判定");
	ImGui::End();
#endif // _DEBUG
}

NodeResult JudgeBarrierStateLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	bool isBarrier = mpBlackBoard->GetValue<bool>("IsBarrier");
	bool isPreBarrier = mpBlackBoard->GetValue<bool>("IsPreBarrier");

	//もし前フレームはバリアが張られていて、現フレームでバリアが壊れていたらsuccessを返す
	if (isPreBarrier && !isBarrier) {
		return NodeResult::Success;
	}

	//それ以外ならfailを返す
    return NodeResult::Fail;
}
