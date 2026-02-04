#include "CannonAttackLeaf.h"
#include <ImGuiManager.h>

//アプリケーション
#include <application/object/character/weapon/enemy/manager/EnemyWeaponManager.h>

using namespace Norm;

CannonAttackLeaf::CannonAttackLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

CannonAttackLeaf::~CannonAttackLeaf() {
}

void CannonAttackLeaf::Update() {
}

void CannonAttackLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("キャノン攻撃");
	ImGui::End();
#endif // _DEBUG
}

NodeResult CannonAttackLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	EnemyWeaponManager* enemyWeaponManager = mpBlackBoard->GetValue<EnemyWeaponManager*>("EnemyWeaponManager");
	Vector3 playerPos = mpBlackBoard->GetValue<Vector3>("PlayerPos");
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	Vector3 bossRotate = mpBlackBoard->GetValue<Vector3>("BossRotate");
	int cannonMagazine = mpBlackBoard->GetValue<int>("CannonMagazine");

	//初期位置を計算
	Vector3 initPos = bossPos;
	initPos.y += -7.5f;
	initPos.x += std::sinf(bossRotate.y) * 35.0f;
	initPos.z += std::cosf(bossRotate.y) * 35.0f;

	//スポーン
	enemyWeaponManager->SpawnCannon(initPos, playerPos);

	//弾倉をデクリメント
	cannonMagazine--;

	//ブラックボードの情報を送信
	mpBlackBoard->SetValue<int>("CannonMagazine", cannonMagazine);

	//成功を返す
	return NodeResult::Success;
}
