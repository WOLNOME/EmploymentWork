#include "CannonReloadLeaf.h"
#include <ImGuiManager.h>

CannonReloadLeaf::CannonReloadLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

CannonReloadLeaf::~CannonReloadLeaf() {
}

void CannonReloadLeaf::Initialize() {
	//基底クラスの初期化
	LeafNodeBase::Initialize();

	//ブラックボードの情報を初期化
	float cannonReloadTime = mpBlackBoard->GetValue<float>("CannonReloadTime");
	mpBlackBoard->SetValue<float>("CannonReloadTimer", cannonReloadTime);
}

void CannonReloadLeaf::Update() {
	//ブラックボードから必要な情報を取得
	float cannonReloadTime = mpBlackBoard->GetValue<float>("CannonReloadTime");
	float cannonReloadTimer = mpBlackBoard->GetValue<float>("CannonReloadTimer");
	int cannonMagazine = mpBlackBoard->GetValue<int>("CannonMagazine");

	//リロードタイマーをデクリメント
	cannonReloadTimer -= kDeltaTime;
	//リロードが終了したら
	if (cannonReloadTimer <= 0.0f) {
		//弾倉をリロード
		cannonMagazine = kCannonMaxMagazine_;
		//リロードタイマーをリセット
		cannonReloadTimer = 0.0f;
	}

	//ブラックボードに更新した情報を保存
	mpBlackBoard->SetValue<float>("CannonReloadTimer", cannonReloadTimer);
	mpBlackBoard->SetValue<int>("CannonMagazine", cannonMagazine);

}

void CannonReloadLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("キャノンリロード");
	ImGui::End();
#endif // _DEBUG
}

NodeResult CannonReloadLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	int cannonMagazine = mpBlackBoard->GetValue<int>("CannonMagazine");

	//もし弾倉が空ならrunningを返す
	if (cannonMagazine < kCannonMaxMagazine_) {
		return NodeResult::Running;
	}
	//弾倉が満タンならsuccessを返す
	if (cannonMagazine >= kCannonMaxMagazine_) {
		return NodeResult::Success;
	}
}
