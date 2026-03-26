#include "CameraSystem.h"
#include <algorithm>
#include <MyMath.h>
#include <TextureManager.h>
#include <SpriteManager.h>

//アプリケーション
#include <application/system/CameraManager.h>

using namespace Norm;

void CameraSystem::Initialize() {
	//状態初期化
	flow_ = Flow::Idle;
	isDirection_ = false;

	//各種タイマー初期化
	moveTimer_ = 0.0f;
	halfBlackOutTimer_ = 0.0f;
	stillnessTimer_ = 0.0f;

	//黒スプライト生成
	uint32_t th = TextureManager::GetInstance()->LoadTexture("black.png");

	blackSprite_ = std::make_unique<Sprite>();
	blackSprite_->Initialize(
		SpriteTag{},
		SpriteManager::GetInstance()->GenerateName("black"),
		Order::Front5,
		th
	);

	//最初は非表示かつ透明
	blackSprite_->SetIsDisplay(false);
	blackSprite_->SetColor({ 1.0f,1.0f,1.0f,0.0f });

	//フェード方向フラグ初期化
	isIn_ = false;
}

void CameraSystem::Update() {
	//外部依存チェック
	assert(cameraManager_ && "カメラマネージャーがセットされていません");
	assert(playerUI_ && "プレイヤーUIがセットされていません");

	//現在のアクティブカメラ取得
	auto* camera = cameraManager_->GetActiveCamera();

	switch (flow_) {

	case Flow::Idle:
	{
		//演出未実行状態
		break;
	}

	case Flow::StartBlackOut:
	{
		//暗転開始フェーズ
		//フェードアウト→フェードインを1回行う

		halfBlackOutTimer_ += kDeltaTime;

		//0から1の範囲で正規化されたフェード係数
		float alpha = std::clamp(halfBlackOutTimer_ / halfBlackOutDuration_, 0.0f, 1.0f);

		if (!isIn_) {
			//フェードアウト処理
			//画面を徐々に黒くする
			blackSprite_->SetIsDisplay(true);
			blackSprite_->SetColor({ 1.0f,1.0f,1.0f,alpha });

			//半暗転時間経過で次フェーズへ
			if (halfBlackOutTimer_ >= halfBlackOutDuration_) {
				halfBlackOutTimer_ = 0.0f;
				isIn_ = true;
			}
		}
		else {
			//フェードイン処理
			//黒から画面を戻す
			float inv = 1.0f - alpha;
			blackSprite_->SetColor({ 1.0f,1.0f,1.0f,inv });

			//暗転終了でカメラ移動へ
			if (halfBlackOutTimer_ >= halfBlackOutDuration_) {
				halfBlackOutTimer_ = 0.0f;
				blackSprite_->SetIsDisplay(false);
				flow_ = Flow::MoveCamera;
			}
		}
		break;
	}

	case Flow::MoveCamera:
	{
		//カメラ補間移動フェーズ

		moveTimer_ += kDeltaTime;

		//補間係数計算
		float t = std::clamp(moveTimer_ / moveDuration_, 0.0f, 1.0f);

		//位置と回転を線形補間
		Vector3 pos = MyMath::Lerp(startPos_, targetPos_, t);
		Vector3 rot = MyMath::Lerp(startRot_, targetRot_, t);

		//カメラに反映
		camera->worldTransform.SetTranslate(pos);
		camera->worldTransform.SetRotate(rot);

		//移動終了で静止フェーズへ
		if (moveTimer_ >= moveDuration_) {
			stillnessTimer_ = 0.0f;
			flow_ = Flow::Stillness;
		}
		break;
	}

	case Flow::Stillness:
	{
		//移動後の演出用静止フェーズ

		stillnessTimer_ += kDeltaTime;

		//指定時間静止したら暗転終了処理へ
		if (stillnessTimer_ >= stillnessDuration_) {
			halfBlackOutTimer_ = 0.0f;
			isIn_ = false;
			flow_ = Flow::EndBlackOut;
		}
		break;
	}

	case Flow::EndBlackOut:
	{
		//演出終了時の暗転フェーズ
		//StartBlackOutと同じくアウト→イン

		halfBlackOutTimer_ += kDeltaTime;

		float alpha = std::clamp(halfBlackOutTimer_ / halfBlackOutDuration_, 0.0f, 1.0f);

		if (!isIn_) {
			//再度画面を黒くする
			blackSprite_->SetIsDisplay(true);
			blackSprite_->SetColor({ 1.0f,1.0f,1.0f,alpha });

			if (halfBlackOutTimer_ >= halfBlackOutDuration_) {
				halfBlackOutTimer_ = 0.0f;
				isIn_ = true;
			}
		}
		else {
			//黒から通常画面に戻す
			float inv = 1.0f - alpha;
			blackSprite_->SetColor({ 1.0f,1.0f,1.0f,inv });

			//演出完全終了
			if (halfBlackOutTimer_ >= halfBlackOutDuration_) {
				blackSprite_->SetIsDisplay(false);
				flow_ = Flow::Idle;
				isDirection_ = false;
				halfBlackOutTimer_ = 0.0f;
			}
		}
		break;
	}
	}
}

void CameraSystem::SetTargetTransform(const Vector3& _pos, const Vector3& _rot) {

	//外部依存チェック
	assert(cameraManager_ && "カメラマネージャーがセットされていません");

	//現在のカメラ状態を取得
	auto* camera = cameraManager_->GetActiveCamera();

	//補間開始地点を記録
	startPos_ = camera->worldTransform.GetTranslate();
	startRot_ = camera->worldTransform.GetRotate();

	//目標地点を設定
	targetPos_ = _pos;
	targetRot_ = _rot;

	//各種タイマーリセット
	moveTimer_ = 0.0f;
	stillnessTimer_ = 0.0f;
	halfBlackOutTimer_ = 0.0f;

	//フェード方向初期化
	isIn_ = false;

	//演出中フラグ有効化
	isDirection_ = true;

	//最初のフェーズへ遷移
	flow_ = Flow::StartBlackOut;
}
