#include "KeepDistanceLeaf.h"
#include <ImGuiManager.h>
#include <algorithm>

KeepDistanceLeaf::KeepDistanceLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

KeepDistanceLeaf::~KeepDistanceLeaf() {
}

void KeepDistanceLeaf::Initialize() {
	//基底クラスの初期化
	LeafNodeBase::Initialize();

	//ブラックボードの情報を初期化
	float keepDistanceTime = mpBlackBoard->GetValue<float>("KeepDistanceTime");
	mpBlackBoard->SetValue<float>("KeepDistanceTimer", keepDistanceTime);
}

void KeepDistanceLeaf::Update() {
	//ブラックボードから必要な情報を取得
	float keepDistanceTimer = mpBlackBoard->GetValue<float>("KeepDistanceTimer");
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	Vector3 bossVelocity = mpBlackBoard->GetValue<Vector3>("BossVelocity");
	float bossMaxSpeed = mpBlackBoard->GetValue<float>("BossMaxSpeed");
	float bossSpeed = mpBlackBoard->GetValue<float>("BossSpeed");
	Vector3 bossRotate = mpBlackBoard->GetValue<Vector3>("BossRotate");
	float bossTurnSpeed = mpBlackBoard->GetValue<float>("BossTurnSpeed");
	Vector3 playerPos = mpBlackBoard->GetValue<Vector3>("PlayerPos");
	float floorFriction = mpBlackBoard->GetValue<float>("FloorFriction");

	//タイマーをデクリメント
	keepDistanceTimer -= kDeltaTime;
	//もしタイマーが0以下なら
	if (keepDistanceTimer <= 0.0f) {
		//タイマーをリセット
		keepDistanceTimer = 0.0f;
		//タイマーの情報をブラックボードに送信
		mpBlackBoard->SetValue<float>("KeepDistanceTimer", keepDistanceTimer);

		return;
	}

	//回転処理
	{
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
	}
	//移動処理
	{
		//現在のボスの向きを求める
		Vector3 currentBossDir = {
			std::sinf(bossRotate.y),
			0.0f,
			std::cosf(bossRotate.y)
		};
		currentBossDir.Normalize();

		//移動量を求める
		Vector3 bossBackward = -currentBossDir;
		bossBackward.Normalize();
		bossVelocity += bossBackward * bossSpeed;

		//摩擦力をかける
		Vector3 frictionDir = -bossVelocity.Normalized();
		Vector3 frictionAccel = frictionDir * floorFriction;
		bossVelocity += frictionAccel * kDeltaTime;

		//移動量の大きさを制限
		if (bossVelocity.Length() > bossMaxSpeed) {
			bossVelocity.Normalize();
			bossVelocity *= bossMaxSpeed;
		}
		//移動量の小ささを制限
		if (Vector3(bossVelocity * kDeltaTime).Length() < 0.01f) {
			bossVelocity = { 0.0f,0.0f,0.0f };
		}

		//速度を加算
		bossPos += bossVelocity * kDeltaTime;
	}

	//ブラックボードに更新した情報を保存
	mpBlackBoard->SetValue<float>("KeepDistanceTimer", keepDistanceTimer);
	mpBlackBoard->SetValue<Vector3>("BossRotate", bossRotate);
	mpBlackBoard->SetValue<Vector3>("BossPos", bossPos);
	mpBlackBoard->SetValue<Vector3>("BossVelocity", bossVelocity);
}

void KeepDistanceLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("距離を取る");
	ImGui::End();
#endif // _DEBUG
}

NodeResult KeepDistanceLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	float keepDistanceTimer = mpBlackBoard->GetValue<float>("KeepDistanceTimer");

	//もしタイマーが0より大きかったらrunningを返す
	if (keepDistanceTimer > 0.0f) {
		return NodeResult::Running;
	}
	//タイマーが0以下ならsuccessを返す
	return NodeResult::Success;
}
