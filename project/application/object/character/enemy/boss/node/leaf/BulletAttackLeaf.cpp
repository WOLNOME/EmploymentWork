#include "BulletAttackLeaf.h"
#include <ImGuiManager.h>

//アプリケーション
#include <application/object/character/weapon/enemy/manager/EnemyWeaponManager.h>

using namespace Norm;

BulletAttackLeaf::BulletAttackLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

BulletAttackLeaf::~BulletAttackLeaf() {
}

void BulletAttackLeaf::Update() {
}

void BulletAttackLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("弾丸攻撃");
	ImGui::End();
#endif // _DEBUG
}

NodeResult BulletAttackLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	EnemyWeaponManager* enemyWeaponManager = mpBlackBoard->GetValue<EnemyWeaponManager*>("EnemyWeaponManager");
	Vector3 playerPos = mpBlackBoard->GetValue<Vector3>("PlayerPos");
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	Vector3 bossRotate = mpBlackBoard->GetValue<Vector3>("BossRotate");
	float bulletIntervalTime = mpBlackBoard->GetValue<float>("BulletIntervalTime");
	float bulletIntervalTimer = mpBlackBoard->GetValue<float>("BulletIntervalTimer");
	int bulletMagazine = mpBlackBoard->GetValue<int>("BulletMagazine");

	//ボスとプレイヤーの距離が160mより離れているならsuccessを返す
	if (Vector3(playerPos - bossPos).Length() > 160.0f) {
		return NodeResult::Success;
	}

	//インターバルタイム
	if (bulletIntervalTimer > 0.0f) {
		bulletIntervalTimer -= kDeltaTime;
		//ブラックボードの情報を送信
		mpBlackBoard->SetValue<float>("BulletIntervalTimer", bulletIntervalTimer);
		//runningを返す
		return NodeResult::Running;
	}

	//初期位置を計算
	float yaw = bossRotate.y;
	Vector3 forward{ std::sinf(yaw), 0.0f, std::cosf(yaw) };
	Vector3 right{ std::cosf(yaw), 0.0f, -std::sinf(yaw) };
	Vector3 up{ 0.0f, 1.0f, 0.0f };
	// 偶数：左砲身 / 奇数：右砲身
	float sideSign = (bulletMagazine % 2 == 0) ? 1.0f : -1.0f;
	// ローカルオフセット（右・下・前）
	Vector3 localOffset{
		14.0f * sideSign,
	   -3.5f,
		31.0f
	};
	// 初期位置
	Vector3 initPos =
		bossPos +
		right * localOffset.x +
		up * localOffset.y +
		forward * localOffset.z;

	//スポーン
	enemyWeaponManager->SpawnBullet(initPos, playerPos);

	//弾倉をデクリメント
	bulletMagazine--;

	//インターバルタイムを設ける
	bulletIntervalTimer = bulletIntervalTime;

	//ブラックボードの情報を送信
	mpBlackBoard->SetValue<float>("BulletIntervalTimer", bulletIntervalTimer);
	mpBlackBoard->SetValue<int>("BulletMagazine", bulletMagazine);


	//弾倉が0より大きいならrunningを返す
	if (bulletMagazine > 0) {
		return NodeResult::Running;
	}
	else {
		//それ以外なら成功を返す
		return NodeResult::Success;
	}
}
