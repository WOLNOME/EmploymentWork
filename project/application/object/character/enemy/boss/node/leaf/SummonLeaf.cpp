#include "SummonLeaf.h"
#include <ImGuiManager.h>

SummonLeaf::SummonLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

SummonLeaf::~SummonLeaf() {
}

void SummonLeaf::Initialize() {
	//基底クラスの初期化
	LeafNodeBase::Initialize();

	//ブラックボードの情報を初期化
	mpBlackBoard->SetValue<bool>("IsSummon", false);
	float summonDirTime = mpBlackBoard->GetValue<float>("SummonDirTime");
	mpBlackBoard->SetValue<float>("SummonDirTimer", summonDirTime);
}

void SummonLeaf::Update() {
	//ブラックボードから必要な情報を取得
	bool isSummon = mpBlackBoard->GetValue<bool>("IsSummon");
	float summonDirTimer = mpBlackBoard->GetValue<float>("SummonDirTimer");
	float summonCoolTime = mpBlackBoard->GetValue<float>("SummonCoolTime");
	float summonCoolTimer = mpBlackBoard->GetValue<float>("SummonCoolTimer");

	//演出タイマーを更新
	summonDirTimer -= kDeltaTime;
	//演出タイマーが0以下になったら召喚を貼る
	if (summonDirTimer <= 0.0f) {
		//召喚する
		isSummon = true;
		//召喚演出タイマーをリセット
		summonDirTimer = 0.0f;
		//召喚のクールタイムをセット
		summonCoolTimer = summonCoolTime;
	}

	//ブラックボードに更新した情報を保存
	mpBlackBoard->SetValue<bool>("IsSummon", isSummon);
	mpBlackBoard->SetValue<float>("SummonDirTimer", summonDirTimer);
	mpBlackBoard->SetValue<float>("SummonCoolTimer", summonCoolTimer);
}

void SummonLeaf::Finalize() {
	//基底クラスの終了処理
	LeafNodeBase::Finalize();
}

void SummonLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("召喚");
	ImGui::End();
#endif // _DEBUG
}

NodeResult SummonLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	bool isSummon = mpBlackBoard->GetValue<bool>("IsSummon");

	//召喚がアクティブでないならrunningを返す
	if (!isSummon) {
		return NodeResult::Running;
	}
	//召喚がアクティブならsuccessを返す
	return NodeResult::Success;
}
