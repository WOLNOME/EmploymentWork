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
}

void EnemyManager::Update() {
	for (const auto& enemy : enemies_) {
		enemy->Update();
	}
}

void EnemyManager::Draw() {
	for (const auto& enemy : enemies_) {
		enemy->Draw();
	}
}

void EnemyManager::DrawLine() {
	for (const auto& enemy : enemies_) {
		enemy->DrawLine();
	}
}

void EnemyManager::DebugWithImGui() {
	for (const auto& enemy : enemies_) {
		enemy->DebugWithImGui();
	}
}

void EnemyManager::SetCamera(GameCamera* _camera) {
	camera_ = _camera;
	for (const auto& enemy : enemies_) {
		enemy->SetCamera(camera_);
	}
}

void EnemyManager::SetLight(SceneLight* _light) {
	sceneLight_ = _light;
	for (const auto& enemy : enemies_) {
		enemy->SetSceneLight(sceneLight_);
	}
}

void EnemyManager::SetPlayer(Player* _player) {
	player_ = _player;
	for (const auto& enemy : enemies_) {
		enemy->SetPlayer(player_);
	}
}
