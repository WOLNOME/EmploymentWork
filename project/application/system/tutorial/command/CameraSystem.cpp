#include "CameraSystem.h"
#include <algorithm>
#include <MyMath.h>

//アプリケーション
#include <application/system/CameraManager.h>

using namespace Norm;

void CameraSystem::Initialize() {
	flow_ = Flow::Idle;
	isDirection_ = false;

	moveTimer_ = 0.0f;
	halfBlackOutTimer_ = 0.0f;
	stillnessTimer_ = 0.0f;

	isOut_ = false;
}

void CameraSystem::Update() {

	if (!cameraManager_) return;

	auto* camera = cameraManager_->GetActiveCamera();

	switch (flow_) {
	case Flow::Idle:
	{
		break;
	}
	case Flow::StartBlackOut:
	{
		halfBlackOutTimer_ += kDeltaTime;

		// 前半：フェードアウト
		if (!isOut_) {
			if (halfBlackOutTimer_ >= halfBlackOutDuration_) {
				halfBlackOutTimer_ = 0.0f;
				isOut_ = true; // 次はフェードイン
			}
		}
		// 後半：フェードイン
		else {
			if (halfBlackOutTimer_ >= halfBlackOutDuration_) {
				halfBlackOutTimer_ = 0.0f;
				flow_ = Flow::MoveCamera;
			}
		}
		break;
	}
	case Flow::MoveCamera:
	{
		moveTimer_ += kDeltaTime;

		{
			float t = std::clamp(moveTimer_ / moveDuration_, 0.0f, 1.0f);

			Vector3 pos = MyMath::Lerp(startPos_, targetPos_, t);
			Vector3 rot = MyMath::Lerp(startRot_, targetRot_, t);

			camera->worldTransform.SetTranslate(pos);
			camera->worldTransform.SetRotate(rot);
		}

		if (moveTimer_ >= moveDuration_) {
			stillnessTimer_ = 0.0f;
			flow_ = Flow::Stillness;
		}
		break;
	}
	case Flow::Stillness:
	{
		stillnessTimer_ += kDeltaTime;

		if (stillnessTimer_ >= stillnessDuration_) {
			halfBlackOutTimer_ = 0.0f;
			isOut_ = false;
			flow_ = Flow::EndBlackOut;
		}
		break;
	}
	case Flow::EndBlackOut:
	{
		halfBlackOutTimer_ += kDeltaTime;

		// 前半：フェードアウト
		if (!isOut_) {
			if (halfBlackOutTimer_ >= halfBlackOutDuration_) {
				halfBlackOutTimer_ = 0.0f;
				isOut_ = true;
			}
		}
		// 後半：フェードイン
		else {
			if (halfBlackOutTimer_ >= halfBlackOutDuration_) {
				flow_ = Flow::Idle;
				isDirection_ = false;
				halfBlackOutTimer_ = 0.0f;
			}
		}
		break;
	}
	}
}

void CameraSystem::SetTargetTransform(Vector3 _pos, Vector3 _rot) {

	if (!cameraManager_) return;

	auto* camera = cameraManager_->GetActiveCamera();

	startPos_ = camera->worldTransform.GetTranslate();
	startRot_ = camera->worldTransform.GetRotate();

	targetPos_ = _pos;
	targetRot_ = _rot;

	moveTimer_ = 0.0f;
	stillnessTimer_ = 0.0f;
	halfBlackOutTimer_ = 0.0f;

	isOut_ = false;
	isDirection_ = true;

	flow_ = Flow::StartBlackOut;
}
