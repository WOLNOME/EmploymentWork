#include "GamePlayScene.h"
#include "SceneManager.h"

void GamePlayScene::Initialize() {
	//シーン共通の初期化
	BaseScene::Initialize();

	//インプットの初期化
	input_ = Input::GetInstance();

	//カメラの生成・初期化
	camera_ = std::make_unique<GameCamera>();
	camera_->Initialize();
	camera_->SetFarClip(2000.0f);
	camera_->worldTransform.rotate = { 0.15f,0.0f,0.0f };
	camera_->worldTransform.translate = { 0.0f,20.0f,-80.0f };

	//ライトの生成・初期化＆登録
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->Initialize();
	dirLight_->direction_ = { 1.0f,-1.0f,1.0f };
	sceneLight_->SetLight(dirLight_.get());

	//インスタンスの生成
	skydome_ = std::make_unique<Skydome>();
	ground_ = std::make_unique<Ground>();
	player_ = std::make_unique<Player>();
	playerWeaponManager_ = std::make_unique<PlayerWeaponManager>();
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyWeaponManager_ = std::make_unique<EnemyWeaponManager>();
	playerUI_ = std::make_unique<PlayerUI>();
	enemyUI_ = std::make_unique<EnemyUI>();
	//インスタンスの初期化
	skydome_->Initialize();
	ground_->Initialize();
	player_->Initialize();
	playerWeaponManager_->Initialize();
	enemyManager_->Initialize();
	enemyWeaponManager_->Initialize();
	playerUI_->Initialize();
	enemyUI_->Initialize();
	//カメラ、ライトのセット
	LineManager::GetInstance()->SetCamera(camera_.get());
	ParticleManager::GetInstance()->SetCamera(camera_.get());
	player_->SetCamera(camera_.get());
	playerWeaponManager_->SetCamera(camera_.get());
	enemyManager_->SetCamera(camera_.get());
	enemyWeaponManager_->SetCamera(camera_.get());
	player_->SetSceneLight(sceneLight_.get());
	playerWeaponManager_->SetLight(sceneLight_.get());
	enemyManager_->SetLight(sceneLight_.get());
	enemyWeaponManager_->SetLight(sceneLight_.get());
	playerUI_->SetCamera(camera_.get());
	enemyUI_->SetCamera(camera_.get());
	//その他インスタンスのセット
	playerWeaponManager_->SetPlayer(player_.get());
	enemyManager_->SetPlayer(player_.get());
	enemyWeaponManager_->SetEnemyManager(enemyManager_.get());
	enemyWeaponManager_->SetPlayer(player_.get());
	playerUI_->SetPlayer(player_.get());
	enemyUI_->SetEnemyManager(enemyManager_.get());

}

void GamePlayScene::Finalize() {
}

void GamePlayScene::Update() {
	//シーン共通の更新
	BaseScene::Update();

	//シーンリセット
	if (input_->TriggerKey(DIK_R)) {
		sceneManager_->SetNextScene("GamePlay");
	}
	//プレイヤーが死亡したら
	if (player_->GetIsDead()) {
		sceneManager_->SetNextScene("GameOver");
	}

	//カメラの更新
	camera_->Update();

	//インスタンスの更新
	skydome_->Update();
	ground_->Update();
	player_->Update();
	playerWeaponManager_->Update();
	enemyManager_->Update();
	enemyWeaponManager_->Update();
	playerUI_->Update();
	enemyUI_->Update();


	//ImGui
#ifdef _DEBUG
	//カメラのImGui
	camera_->DebugWithImGui();

	//プレイヤーのImGui
	player_->DebugWithImGui();
	//プレイヤー武器マネージャーのImGui
	playerWeaponManager_->DebugWithImGui();
	//敵のImGui
	enemyManager_->DebugWithImGui();
	//敵武器マネージャーのImGui
	enemyWeaponManager_->DebugWithImGui();
	//プレイヤーUIのImGui
	playerUI_->DebugWithImGui();
	//敵UIのImGui
	enemyUI_->DebugWithImGui();
	//スプライトのデバッグ
	//SpriteManager::GetInstance()->DebugWithImGui();

#endif // _DEBUG
}

void GamePlayScene::Draw() {
	//3Dモデルの共通描画設定
	Object3dCommon::GetInstance()->SettingCommonDrawing();

	///------------------------------///
	///↓↓↓↓モデル描画開始↓↓↓↓
	///------------------------------///

	skydome_->Draw(*camera_.get());
	ground_->Draw(*camera_.get());
	player_->Draw();
	playerWeaponManager_->Draw();
	enemyManager_->Draw();
	enemyWeaponManager_->Draw();

	///------------------------------///
	///↑↑↑↑モデル描画終了↑↑↑↑
	///------------------------------///
}
