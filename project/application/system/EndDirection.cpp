#include "EndDirection.h"
#include "Input.h"
#include "SceneManager.h"
#include <MyMath.h>
#include <cassert>

//アプリケーション
#include "application/system/TimeScaleManager.h"
#include "application/object/character/player/Player.h"
#include "application/object/character/enemy/manager/EnemyManager.h"

using namespace Norm;

void EndDirection::Initialize() {
	//インプットの取得
	input_ = Input::GetInstance();
	//シーンマネージャーの取得
	sceneManager_ = SceneManager::GetInstance();

	//メンバ変数の初期化
	isSlowMotion_ = false;
	slowMotionTimer_ = 0.0f;

}

void EndDirection::Update() {
	//タイムスケールマネージャーがnullptrでないことを確認
	assert(timeScaleManager_ != nullptr && "タイムスケールマネージャーがセットされていません");
	//プレイヤーインスタンスがnullptrでないことを確認
	assert(player_ != nullptr && "プレイヤーがセットされていません");
	//敵マネージャーインスタンスがnullptrでないことを確認
	assert(enemyManager_ != nullptr && "敵マネージャーがセットされていません");

	//シーン変更処理
	SceneChange();
	//全ボス撃破処理
	AllBossDefeated();

}

void EndDirection::SceneChange() {
	//プレイヤーが死亡したらゲームオーバー
	if (player_->GetState() == BaseCharacter::State::kIdle) {
		uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("black.png");
		sceneManager_->SetNextScene("GameOver", SceneTransitionAnimation::Type::FADE, SceneTransitionAnimation::Type::FADE, SceneTransitionAnimation::Option::NONE, 2.0f, textureHandle);
	}
	//もしスローモーション中で、かつスローモーションタイマーが一定時間を超えたら
	if (isSlowMotion_ && slowMotionTimer_ >= kSlowMotionToSceneChangeTime_) {
		//クリアシーンへ遷移
		uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("white.png");
		sceneManager_->SetNextScene("GameClear", SceneTransitionAnimation::Type::FADE, SceneTransitionAnimation::Type::FADE, SceneTransitionAnimation::Option::NONE, 2.0f, textureHandle);
	}
}

void EndDirection::AllBossDefeated() {
	//ボスを全て倒したかをチェック
	bool allBossDead = true;
	for (auto& keyCanota : enemyManager_->GetKeyCanotas()) {
		if (keyCanota->GetCurrentStateName() == IBaseTankEnemy::StateName::kDead) {
			allBossDead = true;
			break;
		}
		allBossDead = false;
	}
	//もし全てのボスが死亡していたら
	if (allBossDead) {
		//まだスローモーションになっていなければ
		if (!isSlowMotion_) {
			//10秒間スローモーションにする
			timeScaleManager_->ApplyTimeEffect(10.0f, 2);
			//スローモーションフラグをオンにする
			isSlowMotion_ = true;
		}
	}
	//スローモーション中なら
	if (isSlowMotion_) {
		//タイマーを進める
		slowMotionTimer_ += kDeltaTime;
	}
}
