#include "application/EnemyManager.h"
#include <random>

//アプリケーション
#include "application/object/player/Player.h"

void EnemyManager::Initialize() {
	//敵を規定数沸かせる(ここら辺の処理は後々jsonから読み込めるようにする)
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(-350.0f, 350.0f);
	for (int i = 0; i < numEnemies_; i++) {
		std::unique_ptr<Enemy> enemy = nullptr;
		enemy = std::make_unique<Enemy>();
		enemy->Initialize();
		Vector3 initPos = { dist(gen),0.0f,dist(gen) };
		enemy->SetPosition(initPos);
		//登録
		enemies_.push_back(std::move(enemy));
	}
	//ボスの生成処理
	{
		boss_ = std::make_unique<Boss>();
		boss_->Initialize();
		Vector3 initPos = { 00.0f, 0.0f, 400.0f };
		boss_->SetPosition(initPos);
	}

}

void EnemyManager::Update() {
	//全エネミーの死亡時処理
	for (auto it = enemies_.begin(); it != enemies_.end();) {
		if ((*it)->GetIsDead()) {
			it = enemies_.erase(it);
		}
		else {
			++it;
		}
	}
	//ボスの死亡時処理
	if (boss_) {
		if (boss_->GetIsDead()) {
			boss_.reset(); //ボスのインスタンスを解放
			boss_ = nullptr; //ボスが死亡したらnullptrにする
		}
	}
	for (const auto& enemy : enemies_) {
		//全エネミーの更新
		enemy->Update();
	}
	//ボスの更新
	if (boss_)
		boss_->Update();
}

void EnemyManager::Draw() {
	for (const auto& enemy : enemies_) {
		//全エネミーの描画
		enemy->Draw();
	}
	//ボスの描画
	if (boss_)
		boss_->Draw();
}

void EnemyManager::DrawLine() {
	for (const auto& enemy : enemies_) {
		enemy->DrawLine();
	}
	if (boss_)
		boss_->DrawLine();
}

void EnemyManager::DebugWithImGui() {
	for (const auto& enemy : enemies_) {
		enemy->DebugWithImGui();
	}
	if (boss_)
		boss_->DebugWithImGui();
}

void EnemyManager::SetCamera(GameCamera* _camera) {
	camera_ = _camera;
	for (const auto& enemy : enemies_) {
		enemy->SetCamera(camera_);
	}
	if (boss_)
		boss_->SetCamera(camera_);
}

void EnemyManager::SetLight(SceneLight* _light) {
	sceneLight_ = _light;
	for (const auto& enemy : enemies_) {
		enemy->SetSceneLight(sceneLight_);
	}
	if (boss_)
		boss_->SetSceneLight(sceneLight_);
}

void EnemyManager::SetPlayer(Player* _player) {
	player_ = _player;
	for (const auto& enemy : enemies_) {
		enemy->SetPlayer(player_);
	}
	if (boss_)
		boss_->SetPlayer(player_);
}
