#include "SummonLeaf.h"
#include <ImGuiManager.h>
#include <MyMath.h>

//アプリケーション
#include <application/object/character/enemy/manager/EnemyManager.h>

using namespace Norm;

SummonLeaf::SummonLeaf(int _nodeID, BlackBoard* _blackBoard) : LeafNodeBase(_nodeID, _blackBoard) {
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

	//召喚する座標の初期化
	{
		//ブラックボードから必要な情報を取り出す
		Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
		Vector3 bossRotate = mpBlackBoard->GetValue<Vector3>("BossRotate");

		//ボスの左右方向を求める
		Vector3 bossRightDir = {
			std::sinf(bossRotate.y + (1.0f / 2.0f) * pi),
			0.0f,
			std::cosf(bossRotate.y + (1.0f / 2.0f) * pi)
		};
		bossRightDir.Normalize();
		Vector3 bossLeftDir = {
			std::sinf(bossRotate.y - (1.0f / 2.0f) * pi),
			0.0f,
			std::cosf(bossRotate.y - (1.0f / 2.0f) * pi)
		};
		bossLeftDir.Normalize();

		//召喚座標を設定
		summonPositions_[0] = bossPos + bossRightDir * 50.0f;
		summonPositions_[1] = bossPos + bossLeftDir * 50.0f;
	}

}

void SummonLeaf::Update() {
	//ブラックボードから必要な情報を取得
	EnemyManager* enemyManager = mpBlackBoard->GetValue<EnemyManager*>("EnemyManager");
	bool isSummon = mpBlackBoard->GetValue<bool>("IsSummon");
	float summonDirTimer = mpBlackBoard->GetValue<float>("SummonDirTimer");
	float summonCoolTime = mpBlackBoard->GetValue<float>("SummonCoolTime");
	float summonCoolTimer = mpBlackBoard->GetValue<float>("SummonCoolTimer");

	//演出タイマーを更新
	summonDirTimer -= kDeltaTime;

	//演出
	{
		
	}

	//回転処理
	Rotate();

	//演出タイマーが0以下になったら召喚を貼る
	if (summonDirTimer <= 0.0f) {
		//ブラックボードから必要な情報を取得
		Vector3 bossRotate = mpBlackBoard->GetValue<Vector3>("BossRotate");

		//召喚する
		isSummon = true;
		//召喚演出タイマーをリセット
		summonDirTimer = 0.0f;
		//召喚のクールタイムをセット
		summonCoolTimer = summonCoolTime;
		//召喚処理
		enemyManager->CanotaSpawn(summonPositions_[0], bossRotate);
		enemyManager->CanotaSpawn(summonPositions_[1], bossRotate);
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

void SummonLeaf::Rotate() {
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