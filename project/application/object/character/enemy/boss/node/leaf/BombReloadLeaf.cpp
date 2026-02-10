#include "BombReloadLeaf.h"
#include <MyMath.h>
#include <ImGuiManager.h>

using namespace Norm;

BombReloadLeaf::BombReloadLeaf(int _nodeID, BlackBoard* _blackBoard) : LeafNodeBase(_nodeID, _blackBoard) {
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

	//回転処理
	Rotate();

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

void BombReloadLeaf::Rotate() {
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