#include "JudgePlayerPreDistanceLeaf.h"
#include <ImGuiManager.h>

using namespace Norm;

JudgePlayerPreDistanceLeaf::JudgePlayerPreDistanceLeaf(BlackBoard* _blackBoard, float _distance) : LeafNodeBase(_blackBoard), distance_(_distance) {
}

JudgePlayerPreDistanceLeaf::~JudgePlayerPreDistanceLeaf() {
}

void JudgePlayerPreDistanceLeaf::Update() {
}

void JudgePlayerPreDistanceLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("プレイヤー前フレーム距離判定");
	ImGui::End();
#endif // _DEBUG
}

NodeResult JudgePlayerPreDistanceLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	Vector3 bossPrePos = mpBlackBoard->GetValue<Vector3>("BossPrePos");
	bossPrePos.y = 0.0f;
	Vector3 playerPrePos = mpBlackBoard->GetValue<Vector3>("PlayerPrePos");
	playerPrePos.y = 0.0f;

	//前フレームの距離を計算
	float preDist = (bossPrePos - playerPrePos).Length();

	//もし現フレームの距離が設定距離以下＆前フレームの距離が設定距離より大きいならSuccessを返す
	if (preDist > distance_) {
		return NodeResult::Success;
	}

	//前フレームの距離が設定距離より小さいならfailを返す
	return NodeResult::Fail;
}
