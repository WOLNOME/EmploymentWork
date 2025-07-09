#include "Radar.h"
#include <SpriteManager.h>
#include <TextureManager.h>
#include <ImGuiManager.h>

#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>
#include <application/object/character/enemy/manager/EnemyManager.h>

void Radar::Initialize() {
	//スプライトの生成・初期化
	{
		//プレイヤー
		thPlayerMark_ = TextureManager::GetInstance()->LoadTexture("whiteHeart.png");
		playerMark_ = std::make_unique<Sprite>();
		playerMark_->Initialize(SpriteManager::GetInstance()->GenerateName("playerMark"), Sprite::Order::Front1, thPlayerMark_);
		playerMark_->SetAnchorPoint({ 0.5f,0.5f });
		playerMark_->SetSize(playerMark_->GetSize() * 0.2f);
		playerMark_->SetPosition(playerPos);
		playerMark_->SetColor({ 1,0,0,1 });
	}
	{
		//エネミー
		for (int i = 0; i < kEnemyUINum_; i++) {
			thEnemyMarks_[i] = TextureManager::GetInstance()->LoadTexture("whiteHeart.png");
			enemyMarks_[i] = std::make_unique<Sprite>();
			enemyMarks_[i]->Initialize(SpriteManager::GetInstance()->GenerateName("enemyMark"), Sprite::Order::Front1, thEnemyMarks_[i]);
			enemyMarks_[i]->SetAnchorPoint({ 0.5f,0.5f });
			enemyMarks_[i]->SetSize(enemyMarks_[i]->GetSize() * 0.1f);

		}
		
	}
}

void Radar::Update() {
	//プレイヤーがセットされていなければ警告
	assert(player_ != nullptr && "RadarにPlayerインスタンスを渡してください");
	//エネミーマネージャーがセットされていなければ警告
	assert(enemyManager_ != nullptr && "RadarにEnemyManagerインスタンスを渡してください");

	//レーダーにうつるエネミーの座標を更新
	UpdateEnemyUIPosition();

}

void Radar::DebugWithImGui() {
#ifdef _DEBUG

#endif // _DEBUG

}

void Radar::UpdateEnemyUIPosition() {
	//全ての敵スプライトを非表示に
	for (int i = 0; i < kEnemyUINum_; i++) {
		enemyMarks_[i]->SetIsDisplay(false);
	}

	//スプライトのインデックス
	int spriteIndex = 0;

	//キャノ太を回す
	for (const auto& canota : enemyManager_->GetCanotas()) {
		//死亡していたら次の要素へ
		if (canota->GetHP() == 0) {
			continue;
		}

		//プレイヤー→キャノ太のベクトルを求める
		Vector3 playerToCanota = canota->GetWorldPosition() - player_->GetWorldPosition();

		//プレイヤーとキャノ太の距離が索敵距離より大きければ次の要素へ
		if (playerToCanota.Length() > searchLength) {
			continue;
		}

		//座標を決める(xが横軸、zが縦軸)
		enemyMarks_[spriteIndex]->SetPosition({ playerPos.x + (playerToCanota.x * unitLength),playerPos.y + (playerToCanota.z * unitLength) });

		//表示設定
		enemyMarks_[spriteIndex]->SetIsDisplay(true);

		//青色設定
		enemyMarks_[spriteIndex]->SetColor({ 0,0,1,1 });

		//スプライトインデックスのインクリメント
		spriteIndex++;
	}
	//ボスを回す
	for (const auto& boss : enemyManager_->GetBosses()) {
		//死亡していたら次の要素へ
		if (boss->GetHP() == 0) {
			continue;
		}

		//プレイヤー→ボスのベクトルを求める
		Vector3 playerToBoss = boss->GetWorldPosition() - player_->GetWorldPosition();

		//プレイヤーとボスの距離が索敵距離より大きければ次の要素へ
		if (playerToBoss.Length() > searchLength) {
			continue;
		}

		//座標を決める(xが横軸、zが縦軸)
		enemyMarks_[spriteIndex]->SetPosition({ playerPos.x + (playerToBoss.x * unitLength),playerPos.y + (playerToBoss.z * unitLength) });

		//表示設定
		enemyMarks_[spriteIndex]->SetIsDisplay(true);

		//紫色設定
		enemyMarks_[spriteIndex]->SetColor({ 1,0,1,1 });

		//スプライトインデックスのインクリメント
		spriteIndex++;
	}
}
