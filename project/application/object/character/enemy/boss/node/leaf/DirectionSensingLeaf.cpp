#include "DirectionSensingLeaf.h"
#include <ImGuiManager.h>

//アプリケーション
#include <application/ui/enemy/EnemyUI.h>

using namespace Norm;

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

	//ブラックボードから必要な情報を取得
	EnemyUI* enemyUI = mpBlackBoard->GetValue<EnemyUI*>("EnemyUI");
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	float uiHeight = mpBlackBoard->GetValue<float>("UIHeight");
	float uiFront = mpBlackBoard->GetValue<float>("UIFront");

	//発見UIを表示
	enemyUI->GetEnemyReactionUI()->SensingSpawn(bossPos, uiHeight, uiFront);

}

void DirectionSensingLeaf::Update() {
	//ブラックボードから必要な情報を取得
	float sensingTimer = mpBlackBoard->GetValue<float>("SensingDirTimer");

	//発見演出タイマーをデクリメント
	sensingTimer -= kDeltaTime;
	if (sensingTimer < 0.0f) {
		//タイマー
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
