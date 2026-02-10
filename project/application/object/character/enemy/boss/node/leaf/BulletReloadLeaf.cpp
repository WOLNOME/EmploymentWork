#include "BulletReloadLeaf.h"
#include <ImGuiManager.h>

using namespace Norm;

BulletReloadLeaf::BulletReloadLeaf(int _nodeID, BlackBoard* _blackBoard) : LeafNodeBase(_nodeID, _blackBoard) {
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

	//回転処理
	Rotate();


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

void BulletReloadLeaf::Rotate() {
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