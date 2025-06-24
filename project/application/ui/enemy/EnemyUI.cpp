#include "EnemyUI.h"
#include "WinApp.h"
#include "TextureManager.h"
#include <assert.h>

//アプリケーション
#include "application/object/character/enemy/manager/EnemyManager.h"

void EnemyUI::Initialize() {
	//パラメータ読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/enemyUI");
	float hpBarSizeRatio = param_["hpBarSizeRatio"];

	for (int i = 0; i < kNumHPBar; i++) {
		thHPBars_[i][0] = TextureManager::GetInstance()->LoadTexture("hp_redBar.png");
		thHPBars_[i][1] = TextureManager::GetInstance()->LoadTexture("hp_greenBar.png");
		for (int j = 0; j < 2; j++) {
			spriteHPBar_[i][j] = std::make_unique<Sprite>();
			spriteHPBar_[i][j]->Initialize();
			spriteHPBar_[i][j]->AdjustTextureSize(thHPBars_[i][j]);
			spriteHPBar_[i][j]->SetSize({
				spriteHPBar_[i][j]->GetSize().x * hpBarSizeRatio,
				spriteHPBar_[i][j]->GetSize().y * hpBarSizeRatio
				});
		}
	}
	hpBarWidth_ = spriteHPBar_[0][0]->GetSize().x;
	hpBarHeight_ = spriteHPBar_[0][0]->GetSize().y;
}

void EnemyUI::Update() {
	assert(enemyManager_ != nullptr && "EnamyUIにEnemyManagerインスタンスを渡してください");

	//パラメータのローカル変数
	float normalEnemyHPBarHeight = param_["normalEnemyHPBarHeight"];
	float bossEnemyHPBarHeight = param_["bossEnemyHPBarHeight"];

	int hpBarIndex = 0;
	for (int i = 0; i < kNumHPBar; i++) isHPBarVisible_[i] = false;

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
		if (screenPos.x < 0.0f || screenPos.x > WinApp::kClientWidth ||
			screenPos.y < 0.0f || screenPos.y > WinApp::kClientHeight) return;

		Vector2 pos = { screenPos.x - hpBarWidth_ * 0.5f, screenPos.y - hpBarHeight_ * 0.5f };
		isHPBarVisible_[hpBarIndex] = true;
		spriteHPBar_[hpBarIndex][0]->SetPosition(pos);
		spriteHPBar_[hpBarIndex][1]->SetPosition(pos);
		spriteHPBar_[hpBarIndex][1]->SetSize({ hpBarWidth_ * hpRate, spriteHPBar_[hpBarIndex][1]->GetSize().y });
		hpBarIndex++;
		};

	//キャノ太
	for (const auto& canota : enemyManager_->GetCanotas()) {
		Vector3 pos = canota->GetWorldTransform().worldTranslate;
		pos.y += normalEnemyHPBarHeight;
		float rate = (float)canota->GetHP() / (float)canota->GetMaxHP();
		drawHPBar(pos, rate);
	}

	//ボス
	for (const auto& boss : enemyManager_->GetBosses()) {
		Vector3 pos = boss->GetWorldTransform().worldTranslate;
		pos.y += bossEnemyHPBarHeight;
		float rate = (float)boss->GetHP() / (float)boss->GetMaxHP();
		drawHPBar(pos, rate);
	}
}

void EnemyUI::DrawBackSprite() {
	for (int i = 0; i < kNumHPBar; i++) {
		if (!isHPBarVisible_[i]) continue;
		for (int j = 0; j < 2; j++) spriteHPBar_[i][j]->Draw(thHPBars_[i][j]);
	}
}

void EnemyUI::DrawFrontSprite() {
	
}

void EnemyUI::DebugWithImGui() {
#ifdef _DEBUG
	
#endif
}
