#include "DirectionMissingLeaf.h"
#include <ImGuiManager.h>

DirectionMissingLeaf::DirectionMissingLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

DirectionMissingLeaf::~DirectionMissingLeaf() {
}

void DirectionMissingLeaf::Initialize() {
	//基底クラスの初期化
	LeafNodeBase::Initialize();

	//ブラックボードの情報を初期化
	float missingTime = mpBlackBoard->GetValue<float>("MissingDirTime");
	mpBlackBoard->SetValue<float>("MissingDirTimer", missingTime);
}

void DirectionMissingLeaf::Update() {
	//ブラックボードから必要な情報を取得
	float missingTimer = mpBlackBoard->GetValue<float>("MissingDirTimer");

	//見失うタイマーをデクリメント
	missingTimer -= kDeltaTime;
	if (missingTimer < 0.0f) {
		missingTimer = 0.0f;
	}

	//ブラックボードに更新した情報を保存
	mpBlackBoard->SetValue<float>("MissingDirTimer", missingTimer);
}

void DirectionMissingLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("方向見失い演出");
	ImGui::End();
#endif // _DEBUG
}

NodeResult DirectionMissingLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	float missingTimer = mpBlackBoard->GetValue<float>("MissingDirTimer");

	//もし見失うタイマーが0より大きいならrunningを返す
	if (missingTimer > 0.0f) {
		return NodeResult::Running;
	}
	//見失うタイマーが0以下ならsuccessを返す
	return NodeResult::Success;
}
