#include "DirectionConfusionLeaf.h"
#include <ImGuiManager.h>

using namespace Norm;

DirectionConfusionLeaf::DirectionConfusionLeaf(int _nodeID, BlackBoard* _blackBoard) : LeafNodeBase(_nodeID, _blackBoard) {
}

DirectionConfusionLeaf::~DirectionConfusionLeaf() {
}

void DirectionConfusionLeaf::Initialize() {
	//基底クラスの初期化
	LeafNodeBase::Initialize();

	//ブラックボードの情報を初期化
	float confusionTime = mpBlackBoard->GetValue<float>("ConfusionDirTime");
	mpBlackBoard->SetValue<float>("ConfusionDirTimer", confusionTime);

	//ブラックボードから必要な情報を取得
	Vector3 bossRotate = mpBlackBoard->GetValue<Vector3>("BossRotate");

	//変数の初期化
	baseRotate_ = bossRotate;
	cycleTimer_ = kCycleTime_;
	recoveryTimer_ = kRecoveryTime_;

}

void DirectionConfusionLeaf::Update() {
	//ブラックボードから必要な情報を取得
	float confusionTimer = mpBlackBoard->GetValue<float>("ConfusionDirTimer");

	//混乱タイマーをデクリメント
	confusionTimer -= kDeltaTime;
	if (confusionTimer < 0.0f) {
		confusionTimer = 0.0f;
	}

	//混乱処理
	Confusion(confusionTimer);
	//回復処理
	Recovery(confusionTimer);

	//ブラックボードに更新した情報を保存
	mpBlackBoard->SetValue<float>("ConfusionDirTimer", confusionTimer);
}

void DirectionConfusionLeaf::Debug() {
#ifdef _DEBUG
	//現在処理中のノード名を表示
	ImGui::Begin("ボスの稼働中ノード");
	ImGui::Text("方向混乱演出");
	ImGui::End();
#endif // _DEBUG
}

NodeResult DirectionConfusionLeaf::GetNodeResult() const {
	//ブラックボードから必要な情報を取得
	float confusionTimer = mpBlackBoard->GetValue<float>("ConfusionDirTimer");

	//もし混乱タイマーと回復タイマーが両方0.0fでなければRunningを返す
	if (!(confusionTimer == 0.0f && recoveryTimer_ == 0.0f)) {
		return NodeResult::Running;
	}

	//混乱処理、回復処理ともに終了したのならsuccessを返す
	return NodeResult::Success;
}

void DirectionConfusionLeaf::Confusion(float _confusionTimer) {
	//混乱タイマーが終了するまで
	if (_confusionTimer > 0.0f) {

		//回転を定義
		Vector3 rotate = baseRotate_;

		//タイマー更新
		cycleTimer_ -= kDeltaTime;

		//タイマーが規定時間に達したら
		if (cycleTimer_ <= 0.0f) {
			cycleTimer_ = kCycleTime_;
		}

		//角度計算
		float t = 1.0f - (cycleTimer_ / kCycleTime_);
		float angle = t * 2.0f * pi;
		rotate.x = baseRotate_.x + std::cosf(angle) * kAmplitudeX_;
		rotate.y = baseRotate_.y + std::sinf(angle) * kAmplitudeY_;

		//混乱回転の最終的な回転量を取得
		confusionEndRotate_ = rotate;

		//ブラックボードに更新した情報を保存
		mpBlackBoard->SetValue<Vector3>("BossRotate", rotate);

	}
}

void DirectionConfusionLeaf::Recovery(float _confusionTimer) {
	//混乱タイマーが終了していたら
	if (_confusionTimer == 0.0f) {

		//回転を定義
		Vector3 rotate = {};

		//タイマー更新
		recoveryTimer_ -= kDeltaTime;

		//タイマーが規定時間に達したら
		if (recoveryTimer_ <= 0.0f) {
			recoveryTimer_ = 0.0f;
		}

		//角度計算
		float easeT = MyMath::EaseInSine(1.0f - (recoveryTimer_ / kRecoveryTime_));
		rotate = MyMath::Lerp(confusionEndRotate_, baseRotate_, easeT);

		//ブラックボードに更新した情報を保存
		mpBlackBoard->SetValue<Vector3>("BossRotate", rotate);

	}
}
