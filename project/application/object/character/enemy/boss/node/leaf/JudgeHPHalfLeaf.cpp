#include "JudgeHPHalfLeaf.h"
#include <ImGuiManager.h>

using namespace Norm;

JudgeHPHalfLeaf::JudgeHPHalfLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

JudgeHPHalfLeaf::~JudgeHPHalfLeaf() {
}

void JudgeHPHalfLeaf::Update() {
}

void JudgeHPHalfLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("HP半分判定");
	ImGui::End();
#endif // _DEBUG
}

NodeResult JudgeHPHalfLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	int bossMaxHP = mpBlackBoard->GetValue<int>("BossMaxHP");
	int bossHP = mpBlackBoard->GetValue<int>("BossHP");

	//もしHPが半分以下ならsuccessを返す
	if (bossHP <= float(bossMaxHP) / 2.0f) {
		return NodeResult::Success;
	}

	//HPが半分より大きいならfailを返す
	return NodeResult::Fail;
}