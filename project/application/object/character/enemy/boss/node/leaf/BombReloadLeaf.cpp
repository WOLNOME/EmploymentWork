#include "BombReloadLeaf.h"
#include <MyMath.h>
#include <ImGuiManager.h>

//アプリケーション
#include <application/ui/enemy/EnemyUI.h>

using namespace Norm;

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

	//変数の初期化
	isMissing_ = false;
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

	//リアクション処理
	Reaction();

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

void BombReloadLeaf::Reaction() {
	//ブラックボードから必要な情報を取得
	EnemyUI* enemyUI = mpBlackBoard->GetValue<EnemyUI*>("EnemyUI");
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	Vector3 bossPosCopy = bossPos;
	bossPos.y = 0.0f;
	Vector3 bossPrePos = mpBlackBoard->GetValue<Vector3>("BossPrePos");
	bossPrePos.y = 0.0f;
	Vector3 playerPos = mpBlackBoard->GetValue<Vector3>("PlayerPos");
	playerPos.y = 0.0f;
	Vector3 playerPrePos = mpBlackBoard->GetValue<Vector3>("PlayerPrePos");
	playerPrePos.y = 0.0f;
	float uiHeight = mpBlackBoard->GetValue<float>("UIHeight");
	float uiFront = mpBlackBoard->GetValue<float>("UIFront");

	//現フレームと前フレームの距離を求める
	const float kSearchDist = 450.0f;
	float dist = Vector3(bossPos - playerPos).Length();
	float preDist = Vector3(bossPrePos - playerPrePos).Length();

	//一度も見失っていなければ
	if (!isMissing_) {

		//索敵範囲から外れた瞬間
		if (dist > kSearchDist && preDist <= kSearchDist) {
			//見失う演出
			enemyUI->GetEnemyReactionUI()->MissingSpawn(bossPosCopy, uiHeight, uiFront);
			//見失うフラグをオンにする
			isMissing_ = true;
		}

	}
	//一度見つかっていたら
	else {

		//索敵範囲に入った瞬間
		if (dist <= kSearchDist && preDist > kSearchDist) {
			//発見演出
			enemyUI->GetEnemyReactionUI()->SensingSpawn(bossPosCopy, uiHeight, uiFront);
			//見失うフラグをオフにする
			isMissing_ = false;
		}

	}
}
