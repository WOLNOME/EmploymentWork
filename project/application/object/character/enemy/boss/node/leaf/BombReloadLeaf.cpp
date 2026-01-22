#include "BombReloadLeaf.h"
#include <MyMath.h>
#include <ImGuiManager.h>

BombReloadLeaf::BombReloadLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

BombReloadLeaf::~BombReloadLeaf() {
}

void BombReloadLeaf::Initialize() {
	//基底クラスの初期化
	LeafNodeBase::Initialize();

	//ブラックボードの情報を初期化
	float bombReloadTime = mpBlackBoard->GetValue<float>("BombReloadTime");
	mpBlackBoard->SetValue<float>("BombReloadTimer", bombReloadTime);
}

void BombReloadLeaf::Update() {
	//ブラックボードから必要な情報を取得
	float bombReloadTime = mpBlackBoard->GetValue<float>("BombReloadTime");
	float bombReloadTimer = mpBlackBoard->GetValue<float>("BombReloadTimer");
	int bombMagazine = mpBlackBoard->GetValue<int>("BombMagazine");

	//リロードタイマーをデクリメント
	bombReloadTimer -= kDeltaTime;
	//リロードが終了したら
	if (bombReloadTimer <= 0.0f) {
		//弾倉をリロード
		bombMagazine = kBombMaxMagazine_;
		//リロードタイマーをリセット
		bombReloadTimer = 0.0f;
	}

	//ブラックボードに更新した情報を保存
	mpBlackBoard->SetValue<float>("BombReloadTimer", bombReloadTimer);
	mpBlackBoard->SetValue<int>("BombMagazine", bombMagazine);


}

void BombReloadLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("ボムリロード");
	ImGui::End();
#endif // _DEBUG
}

NodeResult BombReloadLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	int bombMagazine = mpBlackBoard->GetValue<int>("BombMagazine");

	//もし弾倉が空ならrunningを返す
	if (bombMagazine < kBombMaxMagazine_) {
		return NodeResult::Running;
	}
	//弾倉が満タンならsuccessを返す
	return NodeResult::Success;

}
