#include "JudgePlayerPreDistanceLeaf.h"]
#include <ImGuiManager.h>

JudgePlayerPreDistanceLeaf::JudgePlayerPreDistanceLeaf(BlackBoard* _blackBoard, float _preDistance) : LeafNodeBase(_blackBoard), preDistance_(_preDistance) {
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
	Vector3 bossPrePos = mpBlackBoard->GetValue<float>("BossPrePos");
	bossPrePos.y = 0.0f;
	Vector3 playerPrePos = mpBlackBoard->GetValue<float>("PlayerPrePos");
	playerPrePos.y = 0.0f;

	//前フレームの距離を計算
	float dist = (bossPrePos - playerPrePos).Length();

	//もし前フレームの距離が設定距離以下ならsuccessを返す
	if (dist <= preDistance_) {
		return NodeResult::Success;
	}

	//前フレームの距離が設定距離より大きいならfailを返す
	return NodeResult::Fail;
}
