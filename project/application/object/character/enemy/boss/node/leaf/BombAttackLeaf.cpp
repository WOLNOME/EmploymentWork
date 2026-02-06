#include "BombAttackLeaf.h"
#include <ImGuiManager.h>

//アプリケーション
#include <application/object/character/weapon/enemy/manager/EnemyWeaponManager.h>

using namespace Norm;

BombAttackLeaf::BombAttackLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

BombAttackLeaf::~BombAttackLeaf() {
}

void BombAttackLeaf::Update() {
}

void BombAttackLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("爆弾攻撃");
	ImGui::End();
#endif // _DEBUG
}

NodeResult BombAttackLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	EnemyWeaponManager* enemyWeaponManager = mpBlackBoard->GetValue<EnemyWeaponManager*>("EnemyWeaponManager");
	Vector3 playerPos = mpBlackBoard->GetValue<Vector3>("PlayerPos");
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	Vector3 bossRotate = mpBlackBoard->GetValue<Vector3>("BossRotate");
	int bombMagazine = mpBlackBoard->GetValue<int>("BombMagazine");

	//初期位置を計算
	float yaw = bossRotate.y;
	Vector3 forward{ std::sinf(yaw), 0.0f, std::cosf(yaw) };
	Vector3 right{ std::cosf(yaw), 0.0f, -std::sinf(yaw) };
	Vector3 up{ 0.0f, 1.0f, 0.0f };
	// ローカルオフセット（右・下・前）
	Vector3 localOffset{
		0.0f,
		26.0f,
		-7.0f
	};
	// 初期位置
	Vector3 initPos =
		bossPos +
		right * localOffset.x +
		up * localOffset.y +
		forward * localOffset.z;

	//スポーン
	enemyWeaponManager->SpawnBomb(BombMethod::Launch, initPos, playerPos);

	//弾倉をデクリメント
	bombMagazine--;

	//ブラックボードの情報を送信
	mpBlackBoard->SetValue<int>("BombMagazine", bombMagazine);

	//成功を返す
	return NodeResult::Success;
}
