#include "BulletReloadLeaf.h"
#include <ImGuiManager.h>

using namespace Norm;

BulletReloadLeaf::BulletReloadLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

BulletReloadLeaf::~BulletReloadLeaf() {
}

void BulletReloadLeaf::Initialize() {
	//基底クラスの初期化
	LeafNodeBase::Initialize();

	//ブラックボードの情報を初期化
	float bulletReloadTime = mpBlackBoard->GetValue<float>("BulletReloadTime");
	mpBlackBoard->SetValue<float>("BulletReloadTimer", bulletReloadTime);
}
void BulletReloadLeaf::Update() {
	//ブラックボードから必要な情報を取得
	float bulletReloadTime = mpBlackBoard->GetValue<float>("BulletReloadTime");
	float bulletReloadTimer = mpBlackBoard->GetValue<float>("BulletReloadTimer");
	int bulletMaxMagazine = mpBlackBoard->GetValue<int>("BulletMaxMagazine");
	int bulletMagazine = mpBlackBoard->GetValue<int>("BulletMagazine");

	//リロードタイマーをデクリメント
	bulletReloadTimer -= kDeltaTime;
	//リロードが終了したら
	if (bulletReloadTimer <= 0.0f) {
		//弾倉をリロード
		bulletMagazine = bulletMaxMagazine;
		//リロードタイマーをリセット
		bulletReloadTimer = 0.0f;
	}

	//ブラックボードに更新した情報を保存
	mpBlackBoard->SetValue<float>("BulletReloadTimer", bulletReloadTimer);
	mpBlackBoard->SetValue<int>("BulletMagazine", bulletMagazine);
}

void BulletReloadLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("弾丸リロード");
	ImGui::End();
#endif // _DEBUG
}

NodeResult BulletReloadLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	int bulletMaxMagazine = mpBlackBoard->GetValue<int>("BulletMaxMagazine");
	int bulletMagazine = mpBlackBoard->GetValue<int>("BulletMagazine");

	//もし弾倉が空ならrunningを返す
	if (bulletMagazine < bulletMaxMagazine) {
		return NodeResult::Running;
	}
	//弾倉が満タンならsuccessを返す
	return NodeResult::Success;
}
