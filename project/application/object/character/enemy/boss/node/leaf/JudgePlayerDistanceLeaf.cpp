#include "JudgePlayerDistanceLeaf.h"
#include <ImGuiManager.h>

using namespace Norm;

JudgePlayerDistanceLeaf::JudgePlayerDistanceLeaf(BlackBoard* _blackBoard, float _distance) : LeafNodeBase(_blackBoard), distance_(_distance) {
}

JudgePlayerDistanceLeaf::~JudgePlayerDistanceLeaf() {
}

void JudgePlayerDistanceLeaf::Update() {
}

void JudgePlayerDistanceLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("プレイヤー距離判定");
	ImGui::End();
#endif // _DEBUG
}

NodeResult JudgePlayerDistanceLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	bossPos.y = 0.0f;
	Vector3 playerPos = mpBlackBoard->GetValue<Vector3>("PlayerPos");
	playerPos.y = 0.0f;
	//ボスとプレイヤーの距離を計算
	float dist = (bossPos - playerPos).Length();

	//もし距離が判定距離以下ならsuccessを返す
	if (dist <= distance_) {
		return NodeResult::Success;
	}

	//距離が判定距離より大きいならfailを返す
	return NodeResult::Fail;
}
