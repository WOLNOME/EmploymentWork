#include "EnemyUI.h"

using namespace Norm;

void EnemyUI::Initialize() {
	//パラメータ読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/enemyUI");

	//HPUIの生成・初期化
	enemyHPUI_ = std::make_unique<EnemyHPUI>();
	enemyHPUI_->Initialize();
	//リアクションUIの生成・初期化
	enemyReactionUI_ = std::make_unique<EnemyReactionUI>();
	enemyReactionUI_->Initialize();

}

void EnemyUI::Update() {
	//HPUIの更新
	enemyHPUI_->Update();
	//リアクションUIの更新
	enemyReactionUI_->Update();
}

void EnemyUI::DebugWithImGui() {
}

void EnemyUI::SetEnemyManager(EnemyManager* _enemyManager) {
	//エネミーマネージャーのセット
	enemyHPUI_->SetEnemyManager(_enemyManager);
}

void EnemyUI::SetGameCamera(GameCamera* _camera) {
	//カメラのセット
	enemyHPUI_->SetGameCamera(_camera);
	enemyReactionUI_->SetGameCamera(_camera);

}