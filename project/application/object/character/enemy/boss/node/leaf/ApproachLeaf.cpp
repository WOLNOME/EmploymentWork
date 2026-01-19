#include "ApproachLeaf.h"
#include <MyMath.h>
#include <ImGuiManager.h>

ApproachLeaf::ApproachLeaf(BlackBoard* _blackBoard) : LeafNodeBase(_blackBoard) {
}

ApproachLeaf::~ApproachLeaf() {
}

void ApproachLeaf::Initialize() {
	//基底クラスの初期化
	LeafNodeBase::Initialize();

}

void ApproachLeaf::Update() {
	//ブラックボードから必要な情報を取得
	Vector3 bossPos = mpBlackBoard->GetValue<Vector3>("BossPos");
	Vector3 bossVelocity = mpBlackBoard->GetValue<Vector3>("BossVelocity");
	float bossMaxSpeed = mpBlackBoard->GetValue<float>("BossMaxSpeed");
	float bossSpeed = mpBlackBoard->GetValue<float>("BossSpeed");
	Vector3 bossRotate = mpBlackBoard->GetValue<Vector3>("BossRotate");
	float bossTurnSpeed = mpBlackBoard->GetValue<float>("BossTurnSpeed");
	Vector3 playerPos = mpBlackBoard->GetValue<Vector3>("PlayerPos");
	float floorFriction = mpBlackBoard->GetValue<float>("FloorFriction");


	//ボスの前方向ベクトルを計算
	Vector3 bossForward;
	bossForward.x = std::cos(bossRotate.y);
	bossForward.y = 0.0f;
	bossForward.z = std::sin(bossRotate.y);

	//プレイヤーへの方向ベクトルを計算
	Vector3 toPlayer = playerPos - bossPos;
	toPlayer.y = 0.0f;
	toPlayer.Normalize();

	//2つのベクトルのなす角を計算
	float angle = MyMath::AngleOf2VectorY(bossForward, toPlayer);

	//angleの絶対値が90°を超えていたらプレイヤーは後ろにいると判断
	bool playerBehind = false;
	if (std::abs(angle) > 1.0f / 2.0f * pi) {
		playerBehind = true;
	}

	//もしプレイヤーが後ろにいるならボスはただ回転させる
	if (playerBehind) {
		//回転方向を決定
		float turnDirection = (angle > 0.0f) ? 1.0f : -1.0f;
		//回転量を計算
		float turnAmount = bossTurnSpeed * turnDirection * kDeltaTime;
		//回転量をクランプ
		if (std::abs(turnAmount) > std::abs(angle)) {
			turnAmount = angle;
		}
		//回転を更新
		bossRotate.y += turnAmount;
		//ブラックボードに更新した回転を保存
		mpBlackBoard->SetValue<Vector3>("BossRotate", bossRotate);
		return;
	}
	else {
		//回転方向を決定
		float turnDirection = (angle > 0.0f) ? 1.0f : -1.0f;
		//回転量を計算
		float turnAmount = bossTurnSpeed * turnDirection * kDeltaTime;
		//回転量をクランプ
		if (std::abs(turnAmount) > std::abs(angle)) {
			turnAmount = angle;
		}
		//回転を更新
		bossRotate.y += turnAmount;

		//移動量を求める
		bossVelocity += bossForward * bossSpeed * kDeltaTime;

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

		//ブラックボードに更新した値を保存
		mpBlackBoard->SetValue<Vector3>("BossRotate", bossRotate);
		mpBlackBoard->SetValue<Vector3>("BossPos", bossPos);
		mpBlackBoard->SetValue<Vector3>("BossVelocity", bossVelocity);

		return;
	}

}

void ApproachLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("接近");
	ImGui::End();
#endif // _DEBUG
}

NodeResult ApproachLeaf::GetNodeResult() const {
	//常に成功を返す
	return NodeResult::Success;
}
