#include "EnemyHPUI.h"
#include <WinApp.h>
#include <TextureManager.h>
#include <SpriteManager.h>
#include <GameCamera.h>
#include <cassert>

//アプリケーション
#include <application/object/character/enemy/manager/EnemyManager.h>

void EnemyHPUI::Initialize() {
	//パラメータ読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/enemyUI");
	float hpBarSizeRatio = param_["hpBarSizeRatio"];

	textureHandles_[0] = TextureManager::GetInstance()->LoadTexture("hp_redBar.png");
	textureHandles_[1] = TextureManager::GetInstance()->LoadTexture("hp_greenBar.png");
	for (int i = 0; i < kNumHPBar; i++) {
		for (int j = 0; j < 2; j++) {
			sprites_[i][j] = std::make_unique<Sprite>();
			if (j == 0)
				sprites_[i][j]->Initialize(SpriteManager::GetInstance()->GenerateName("EnemyHPBar"), Sprite::Order::Back0, textureHandles_[j]);
			else
				sprites_[i][j]->Initialize(SpriteManager::GetInstance()->GenerateName("EnemyHPBar"), Sprite::Order::Back1, textureHandles_[j]);
			sprites_[i][j]->SetSize({
				sprites_[i][j]->GetSize().x * hpBarSizeRatio,
				sprites_[i][j]->GetSize().y * hpBarSizeRatio
				});
		}
	}
	hpBarWidth_ = sprites_[0][0]->GetSize().x;
	hpBarHeight_ = sprites_[0][0]->GetSize().y;
}

void EnemyHPUI::Update() {
	//カメラチェック
	assert(camera_ != nullptr && "カメラをセットしてください");
	//エネミーマネージャーチェック
	assert(enemyManager_ != nullptr && "エネミーマネージャーをセットしてください");

	//パラメータのローカル変数
	float normalEnemyHPBarHeight = param_["normalEnemyHPBarHeight"];
	float bossEnemyHPBarHeight = param_["bossEnemyHPBarHeight"];

	int hpBarIndex = 0;
	for (int i = 0; i < kNumHPBar; i++) {
		//あらかじめ全て非表示状態にしておく
		sprites_[i][0]->SetIsDisplay(false);
		sprites_[i][1]->SetIsDisplay(false);
	}

	//共通描画用ラムダ式
	auto drawHPBar = [&](const Vector3& worldPos, float hpRate) {
		//HPバーの表示率が0以下なら描画しない
		if (hpBarIndex >= kNumHPBar) return;

		Vector3 clipPos = MyMath::Transform(worldPos, camera_->GetViewProjectionMatrix());
		//クリップ座標のZ値が0未満または1より大きいなら描画しない
		if (clipPos.z < 0.0f || clipPos.z > 1.0f) return;

		Vector2 screenPos = {
			(clipPos.x * 0.5f + 0.5f) * WinApp::kClientWidth,
			(clipPos.y * -0.5f + 0.5f) * WinApp::kClientHeight
		};

		//スクリーン座標が画面範囲から外れていたら描画しない
		if (screenPos.x < 0.0f || screenPos.x > WinApp::kClientWidth ||
			screenPos.y < 0.0f || screenPos.y > WinApp::kClientHeight) return;

		Vector2 pos = { screenPos.x - hpBarWidth_ * 0.5f, screenPos.y - hpBarHeight_ * 0.5f };
		//非表示状態を解除
		sprites_[hpBarIndex][0]->SetIsDisplay(true);
		sprites_[hpBarIndex][1]->SetIsDisplay(true);
		//パラメーターを代入
		sprites_[hpBarIndex][0]->SetPosition(pos);
		sprites_[hpBarIndex][1]->SetPosition(pos);
		sprites_[hpBarIndex][1]->SetSize({ hpBarWidth_ * hpRate, sprites_[hpBarIndex][1]->GetSize().y });
		hpBarIndex++;	//←先着方式
		};

	//キャノ太
	for (const auto& canota : enemyManager_->GetCanotas()) {
		//死亡していたら描画しない
		if (canota->GetDeadTimer() > 0.0f || canota->GetIsDead()) return;

		Vector3 pos = canota->GetWorldTransform().worldTranslate;
		pos.y += normalEnemyHPBarHeight;
		float rate = (float)canota->GetHP() / (float)canota->GetMaxHP();
		drawHPBar(pos, rate);
	}

	//ボス
	for (const auto& boss : enemyManager_->GetBosses()) {
		//死亡していたら描画しない
		if (boss->GetDeadTimer() > 0.0f || boss->GetIsDead()) return;

		Vector3 pos = boss->GetWorldTransform().worldTranslate;
		pos.y += bossEnemyHPBarHeight;
		float rate = (float)boss->GetHP() / (float)boss->GetMaxHP();
		drawHPBar(pos, rate);
	}

	//ジェット
	for (const auto& jet : enemyManager_->GetJets()) {
		//死亡していたら描画しない
		if (jet->GetDeadTimer() > 0.0f || jet->GetIsDead())
			return;

		Vector3 pos = jet->GetWorldTransform().worldTranslate;
		pos.y += normalEnemyHPBarHeight;
		float rate = (float)jet->GetHP() / (float)jet->GetMaxHP();
		drawHPBar(pos, rate);
	}
}