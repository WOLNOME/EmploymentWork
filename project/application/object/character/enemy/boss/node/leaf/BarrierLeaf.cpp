#include "BarrierLeaf.h"
#include "ImGuiManager.h"

using namespace Norm;

BarrierLeaf::BarrierLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

BarrierLeaf::~BarrierLeaf() {
}

void BarrierLeaf::Initialize() {
	//基底クラスの初期化
	LeafNodeBase::Initialize();

	//ブラックボードの情報を初期化
	mpBlackBoard->SetValue<bool>("IsBarrier", false);
	float barrierDirTime = mpBlackBoard->GetValue<float>("BarrierDirTime");
	mpBlackBoard->SetValue<float>("BarrierDirTimer", barrierDirTime);
}
void BarrierLeaf::Update() {
	//ブラックボードから必要な情報を取得
	bool isBarrier = mpBlackBoard->GetValue<bool>("IsBarrier");
	float barrierDirTimer = mpBlackBoard->GetValue<float>("BarrierDirTimer");
	float barrierCoolTime = mpBlackBoard->GetValue<float>("BarrierCoolTime");
	float barrierCoolTimer = mpBlackBoard->GetValue<float>("BarrierCoolTimer");
	int barrierHP = mpBlackBoard->GetValue<int>("BarrierHP");
	int barrierMaxHP = mpBlackBoard->GetValue<int>("BarrierMaxHP");

	//演出タイマーを更新
	barrierDirTimer -= kDeltaTime;
	//演出タイマーが0以下になったらバリアを貼る
	if (barrierDirTimer <= 0.0f) {
		//バリアを貼る
		isBarrier = true;
		//バリア演出タイマーをリセット
		barrierDirTimer = 0.0f;
		//バリアのクールタイムをセット
		barrierCoolTimer = barrierCoolTime;
		//バリアのHPを最大にセット
		barrierHP = barrierMaxHP;
	}

	//ブラックボードに更新した情報を保存
	mpBlackBoard->SetValue<bool>("IsBarrier", isBarrier);
	mpBlackBoard->SetValue<float>("BarrierDirTimer", barrierDirTimer);
	mpBlackBoard->SetValue<float>("BarrierCoolTimer", barrierCoolTimer);
	mpBlackBoard->SetValue<int>("BarrierHP", barrierHP);
}

void BarrierLeaf::Finalize() {
	//基底クラスの終了処理
	LeafNodeBase::Finalize();
}

void BarrierLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("バリア");
	ImGui::End();
#endif // _DEBUG
}

NodeResult BarrierLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	bool isBarrier = mpBlackBoard->GetValue<bool>("IsBarrier");

	//バリアがアクティブでないならrunningを返す
	if (!isBarrier) {
		return NodeResult::Running;
	}
	//バリアがアクティブならsuccessを返す
	return NodeResult::Success;
}
