#include "BulletAttackLeaf.h"
#include <ImGuiManager.h>

//アプリケーション
#include <application/object/character/weapon/enemy/manager/EnemyWeaponManager.h>

using namespace Norm;

BulletAttackLeaf::BulletAttackLeaf(int _nodeID, BlackBoard* _blackBoard) : LeafNodeBase(_nodeID, _blackBoard) {
}

BulletAttackLeaf::~BulletAttackLeaf() {
	//ブラックボードから必要な情報を取得
	EnemyWeaponManager* enemyWeaponManager = mpBlackBoard->GetValue<EnemyWeaponManager*>("EnemyWeaponManager");
	Vector3 playerPos = mpBlackBoard->GetValue<Vector3>("PlayerPos");
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	Vector3 bossRotate = mpBlackBoard->GetValue<Vector3>("BossRotate");
	float bulletIntervalTime = mpBlackBoard->GetValue<float>("BulletIntervalTime");
	float bulletIntervalTimer = mpBlackBoard->GetValue<float>("BulletIntervalTimer");
	int bulletMagazine = mpBlackBoard->GetValue<int>("BulletMagazine");

	//回転処理
	Rotate();

	//インターバルタイム
	bulletIntervalTimer -= kDeltaTime;
	if (bulletIntervalTimer > 0.0f) {
		//ブラックボードの情報を送信
		mpBlackBoard->SetValue<float>("BulletIntervalTimer", bulletIntervalTimer);
		
		return;
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
	Vector3 playerPos = mpBlackBoard->GetValue<Vector3>("PlayerPos");
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	float bulletIntervalTimer = mpBlackBoard->GetValue<float>("BulletIntervalTimer");
	int bulletMagazine = mpBlackBoard->GetValue<int>("BulletMagazine");

	//ボスとプレイヤーの距離が160mより離れているならfailを返す（中断）
	if (Vector3(playerPos - bossPos).Length() > 160.0f) {
		return NodeResult::Fail;
	}

	//インターバル中ならrunningを返す
	if (bulletIntervalTimer > 0.0f) {
		//runningを返す
		return NodeResult::Running;
	}

	//弾倉が0より大きいならrunningを返す
	if (bulletMagazine > 0) {
		return NodeResult::Running;
	}
	else {
		//それ以外なら成功を返す
		return NodeResult::Success;
	}
}


void BulletAttackLeaf::Rotate() {
	//ブラックボードから必要な情報を取得
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	Vector3 bossRotate = mpBlackBoard->GetValue<Vector3>("BossRotate");
	float bossTurnSpeed = mpBlackBoard->GetValue<float>("BossTurnSpeed");
	Vector3 playerPos = mpBlackBoard->GetValue<Vector3>("PlayerPos");

	//ボスの回転速度は半減で使う
	bossTurnSpeed *= 0.5f;

	//現在のボスの向きを求める
	Vector3 currentBossDir = {
		std::sinf(bossRotate.y),
		0.0f,
		std::cosf(bossRotate.y)
	};
	currentBossDir.Normalize();
	//目標ポイント（プレイヤーの位置）への方向を求める
	Vector3 targetDir = playerPos - bossPos;
	targetDir.Normalize();
	//回転の差を求める
	float angle = std::atan2f(targetDir.x, targetDir.z) - std::atan2f(currentBossDir.x, currentBossDir.z);
	//angleを-pi~piでクランプする
	if (angle > pi) {
		angle -= 2 * pi;
	}
	else if (angle < -pi) {
		angle += 2 * pi;
	}
	//angle<回転速度の場合
	float usingRotateSpeed = 0.0f;
	if (std::abs(angle) < bossTurnSpeed * kDeltaTime) {
		//仕上げの角度加算
		usingRotateSpeed = angle;
	}
	else {
		//回転速度を使う場合、符号を揃える
		usingRotateSpeed = (angle > 0) ? bossTurnSpeed * kDeltaTime : -bossTurnSpeed * kDeltaTime;
	}
	//回転加算
	bossRotate.y += usingRotateSpeed;
	//-π~πにクランプ
	if (bossRotate.y > pi) {
		bossRotate.y -= 2.0f * pi;
	}
	else if (bossRotate.y < -pi) {
		bossRotate.y += 2.0f * pi;
	}

	//ブラックボードに更新した情報を保存
	mpBlackBoard->SetValue<Vector3>("BossRotate", bossRotate);
}