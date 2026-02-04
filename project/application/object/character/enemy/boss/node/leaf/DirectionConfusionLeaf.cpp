#include "DirectionConfusionLeaf.h"
#include <ImGuiManager.h>

using namespace Norm;

DirectionConfusionLeaf::DirectionConfusionLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

DirectionConfusionLeaf::~DirectionConfusionLeaf() {
}

void DirectionConfusionLeaf::Initialize() {
	//基底クラスの初期化
	LeafNodeBase::Initialize();

	//ブラックボードの情報を初期化
	float confusionTime = mpBlackBoard->GetValue<float>("ConfusionDirTime");
	mpBlackBoard->SetValue<float>("ConfusionDirTimer", confusionTime);
}

void DirectionConfusionLeaf::Update() {
	//ブラックボードから必要な情報を取得
	float confusionTimer = mpBlackBoard->GetValue<float>("ConfusionDirTimer");

	//混乱タイマーをデクリメント
	confusionTimer -= kDeltaTime;
	if (confusionTimer < 0.0f) {
		confusionTimer = 0.0f;
	}

	//ブラックボードに更新した情報を保存
	mpBlackBoard->SetValue<float>("ConfusionDirTimer", confusionTimer);
}

void DirectionConfusionLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("方向混乱演出");
	ImGui::End();
#endif // _DEBUG
}

NodeResult DirectionConfusionLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	float confusionTimer = mpBlackBoard->GetValue<float>("ConfusionDirTimer");

	//もし混乱タイマーが0より大きいならrunningを返す
	if (confusionTimer <= 0.0f) {
		return NodeResult::Running;
	}
	//混乱タイマーが0以下ならsuccessを返す
	return NodeResult::Success;
}
