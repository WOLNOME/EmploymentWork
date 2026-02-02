#include "DirectionSensingLeaf.h"
#include <ImGuiManager.h>

DirectionSensingLeaf::DirectionSensingLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

DirectionSensingLeaf::~DirectionSensingLeaf() {
}

void DirectionSensingLeaf::Initialize() {
	//基底クラスの初期化
	LeafNodeBase::Initialize();

	//ブラックボードの情報を初期化
	float sensingTime = mpBlackBoard->GetValue<float>("SensingDirTime");
	mpBlackBoard->SetValue<float>("SensingDirTimer", sensingTime);
}

void DirectionSensingLeaf::Update() {
	//ブラックボードから必要な情報を取得
	float sensingTimer = mpBlackBoard->GetValue<float>("SensingDirTimer");

	//発見演出タイマーをデクリメント
	sensingTimer -= kDeltaTime;
	if (sensingTimer < 0.0f) {
		sensingTimer = 0.0f;
	}

	//ブラックボードに更新した情報を保存
	mpBlackBoard->SetValue<float>("SensingDirTimer", sensingTimer);
}

void DirectionSensingLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("方向発見演出");
	ImGui::End();
#endif // _DEBUG
}

NodeResult DirectionSensingLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	float sensingTimer = mpBlackBoard->GetValue<float>("SensingDirTimer");

	//もし発見演出タイマーが0より大きいならrunningを返す
	if (sensingTimer > 0.0f) {
		return NodeResult::Running;
	}
	//発見演出タイマーが0以下ならrunningを返す
	return NodeResult::Success;
}
