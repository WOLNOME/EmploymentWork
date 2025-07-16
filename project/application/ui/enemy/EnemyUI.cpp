#include "EnemyUI.h"

void EnemyUI::Initialize() {
	//パラメータ読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/enemyUI");

	//HPUIの生成・初期化
	enemyHPUI_ = std::make_unique<EnemyHPUI>();
	enemyHPUI_->Initialize();
}

void EnemyUI::Update() {
	//HPUIの更新
	enemyHPUI_->Update();
}

void EnemyUI::DebugWithImGui() {
#ifdef _DEBUG

#endif
}

void EnemyUI::SetEnemyManager(EnemyManager* _enemyManager) {
	enemyHPUI_->SetEnemyManager(_enemyManager);
}

void EnemyUI::SetGameCamera(GameCamera* _camera) {
	enemyHPUI_->SetGameCamera(_camera);
}