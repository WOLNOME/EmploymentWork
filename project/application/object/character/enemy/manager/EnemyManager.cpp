#include "EnemyManager.h"
#include <random>

//アプリケーション
#include "application/object/character/player/Player.h"

void EnemyManager::Initialize() {
	//敵を規定数沸かせる(ここら辺の処理は後々jsonから読み込めるようにする)
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(-350.0f, 350.0f);
	//キャノ太の生成・初期化処理
	for (int i = 0; i < numCanotas_; i++) {
		std::unique_ptr<Canota> canota = nullptr;
		canota = std::make_unique<Canota>();
		canota->Initialize();
		Vector3 initPos = { dist(gen),2.7f,dist(gen) };
		canota->SetTranslate(initPos);
		//登録
		canotas_.push_back(std::move(canota));
	}
	//ボスの生成・初期化処理
	for (int i = 0; i < numBosses_; i++) {
		std::unique_ptr<Boss> boss = nullptr;
		boss = std::make_unique<Boss>();
		boss->Initialize();
		Vector3 initPos = { 0.0f, 4.5f, 400.0f };
		boss->SetTranslate(initPos);
		//登録
		bosses_.push_back(std::move(boss));
	}

}

void EnemyManager::Update() {
	//全キャノ太の死亡時処理
	for (auto it = canotas_.begin(); it != canotas_.end();) {
		if ((*it)->GetIsDead()) {
			it = canotas_.erase(it);
		}
		else {
			++it;
		}
	}
	//全ボスの死亡時処理
	for (auto it = bosses_.begin(); it != bosses_.end();) {
		if ((*it)->GetIsDead()) {
			it = bosses_.erase(it);
		}
		else {
			++it;
		}
	}

	//全キャノ太の更新
	for (const auto& canota : canotas_) {
		canota->Update();
	}
	//全ボスの更新
	for (const auto& boss : bosses_) {
		boss->Update();
	}
}

void EnemyManager::DebugWithImGui() {
#ifdef _DEBUG
	//全キャノ太のデバッグ処理
	for (const auto& canota : canotas_) {
		canota->DebugWithImGui();
	}
	//全ボスのデバッグ処理
	for (const auto& boss : bosses_) {
		boss->DebugWithImGui();
	}
#endif // _DEBUG
}

void EnemyManager::SetPlayer(Player* _player) {
	player_ = _player;
	for (const auto& canota : canotas_) {
		canota->SetPlayer(player_);
	}
	for (const auto& boss : bosses_) {
		boss->SetPlayer(player_);
	}
}
