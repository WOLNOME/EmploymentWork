#include "TutorialSystem.h"
#include <TextureManager.h>
#include <SpriteManager.h>
#include <SceneManager.h>
#include <Input.h>

//アプリケーション
#include <application/system/CameraManager.h>

using namespace Norm;

void TutorialSystem::Initialize() {

	//BGMの初期化
	bgm_ = std::make_unique<Audio>();
	bgm_->Initialize("bgm/tutorial.mp3");
	bgm_->Play(true, 1.0f);

	//インスタンスの生成
	timeScaleManager_ = std::make_unique<TimeScaleManager>();
	levelLoader_ = std::make_unique<LevelLoader>();
	player_ = std::make_unique<Player>();
	playerWeaponManager_ = std::make_unique<PlayerWeaponManager>();
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyWeaponManager_ = std::make_unique<EnemyWeaponManager>();
	itemManager_ = std::make_unique<ItemManager>();
	playerUI_ = std::make_unique<PlayerUI>();
	enemyUI_ = std::make_unique<EnemyUI>();
	messageUI_ = std::make_unique<MessageUI>();
	pauseSystem_ = std::make_unique<Tuto_PauseSystem>();
	tutorialManager_ = std::make_unique<TutorialManager>();

	//インスタンスの初期化
	timeScaleManager_->Initialize();
	levelLoader_->Initialize("Resources/levelData/tutorialLevelData");
	player_->Initialize();
	playerWeaponManager_->Initialize();
	enemyManager_->Initialize();
	enemyWeaponManager_->Initialize();
	itemManager_->Initialize();
	playerUI_->Initialize();
	enemyUI_->Initialize();
	messageUI_->Initialize();
	pauseSystem_->Initialize();
	tutorialManager_->Initialize();

	//インスタンスのセット
	player_->SetPlayerWeaponManager(playerWeaponManager_.get());
	player_->SetMessageUI(messageUI_.get());
	enemyManager_->SetPlayer(player_.get());
	enemyManager_->SetItemManager(itemManager_.get());
	enemyManager_->SetEnemyWeaponManager(enemyWeaponManager_.get());
	enemyManager_->SetMessageUI(messageUI_.get());
	enemyManager_->SetEnemyUI(enemyUI_.get());
	playerWeaponManager_->SetPlayer(player_.get());
	enemyWeaponManager_->SetEnemyManager(enemyManager_.get());
	enemyWeaponManager_->SetPlayer(player_.get());
	enemyWeaponManager_->SetPlayerUI(playerUI_.get());
	playerUI_->SetPlayer(player_.get());
	playerUI_->SetEnemyManager(enemyManager_.get());
	playerUI_->SetItemManager(itemManager_.get());
	enemyUI_->SetEnemyManager(enemyManager_.get());
	tutorialManager_->SetItemManager(itemManager_.get());
	tutorialManager_->SetPlayer(player_.get());
	tutorialManager_->SetPlayerUI(playerUI_.get());

	//チュートリアルスクリプトをロード
	tutorialManager_->LoadFromFile("Resources/scripts/tutorial_script");
	//初めから再生
	tutorialManager_->Start("tutorial_intro");

}

void TutorialSystem::Update() {
	//タイムスケールマネージャーの更新
	timeScaleManager_->Update();
	//ポーズシステムの更新
	pauseSystem_->Update();

	//タイムスケールマネージャーによる再生速度の管理
	if (!timeScaleManager_->GetIsPlay()) {
		return;
	}
	//ポーズ画面による再生の管理
	if (pauseSystem_->GetIsPause()) {
		//プレイヤーの移動音を停止
		if (player_->GetMoveSE()->GetIsPlaying()) {
			player_->GetMoveSE()->Pause();
		}

		return;
	}
	else {
		player_->GetMoveSE()->Resume();
	}

	//メッセージUIの更新
	messageUI_->Update();

	//インスタンスの更新
	levelLoader_->Update();
	playerUI_->Update();
	player_->Update();
	enemyManager_->Update();
	playerWeaponManager_->Update();
	enemyWeaponManager_->Update();
	itemManager_->Update();
	tutorialManager_->Update();

	//カメラの更新後の処理（スクリーン座標を参照したいインスタンスの更新）
	enemyUI_->Update();
}

void TutorialSystem::DebugWithImGui() {

}

void TutorialSystem::SetCameraManager(CameraManager* _cameraManager) {
	player_->SetCameraManager(_cameraManager);
	playerWeaponManager_->SetCameraManager(_cameraManager);
	playerUI_->SetCameraManager(_cameraManager);
	enemyUI_->SetCameraManager(_cameraManager);
	enemyManager_->SetCameraManager(_cameraManager);
	enemyWeaponManager_->SetCameraManager(_cameraManager);
	tutorialManager_->SetCameraManager(_cameraManager);

	player_->SetLevelLoader(levelLoader_.get());
	enemyManager_->SetLevelLoader(levelLoader_.get());
}