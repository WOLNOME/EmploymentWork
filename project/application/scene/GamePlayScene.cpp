#include "GamePlayScene.h"
#include "SceneManager.h"
#include <TextureManager.h>

using namespace Norm;

void GamePlayScene::Initialize() {
	//シーン共通の初期化
	BaseScene::Initialize();

	//インプットの初期化
	input_ = Input::GetInstance();
	input_->SetIsMouseDisplay(false);
	input_->SetIsMouseFixed(true);

	//カメラの生成・初期化
	camera_ = std::make_unique<GameCamera>();
	camera_->Initialize();
	camera_->SetFarClip(2000.0f);
	camera_->worldTransform.SetRotate({ 0.15f,0.0f,0.0f });
	camera_->worldTransform.SetTranslate({ 0.0f,20.0f,-80.0f });

	//ライトの生成・初期化＆登録
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->Initialize();
	dirLight_->SetDirection({ 1.0f,-1.0f,1.0f });
	sceneLight_->SetLight(dirLight_.get());

	//タイムスケールマネージャーの生成・初期化
	timeScaleManager_ = std::make_unique<TimeScaleManager>();
	timeScaleManager_->Initialize();

	//スタート演出の生成・初期化
	startDirection_ = std::make_unique<StartDirection>();
	startDirection_->Initialize();

	//エンド演出の生成・初期化
	endDirection_ = std::make_unique<EndDirection>();
	endDirection_->Initialize();

	//インスタンスの生成
	skydome_ = std::make_unique<Skydome>();
	ground_ = std::make_unique<Ground>();
	levelLoader_ = std::make_unique<LevelLoader>();

	player_ = std::make_unique<Player>();
	playerWeaponManager_ = std::make_unique<PlayerWeaponManager>();
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyWeaponManager_ = std::make_unique<EnemyWeaponManager>();
	itemManager_ = std::make_unique<ItemManager>();
	playerUI_ = std::make_unique<PlayerUI>();
	enemyUI_ = std::make_unique<EnemyUI>();
	messageUI_ = std::make_unique<MessageUI>();
	pauseSystem_ = std::make_unique<PauseSystem>();

	//インスタンスの初期化
	skydome_->Initialize();
	ground_->Initialize();
	levelLoader_->Initialize();

	player_->Initialize();
	playerWeaponManager_->Initialize();
	enemyManager_->Initialize();
	enemyWeaponManager_->Initialize();
	itemManager_->Initialize();
	playerUI_->Initialize();
	enemyUI_->Initialize();
	messageUI_->Initialize();
	pauseSystem_->Initialize();

	//カメラのセット
	Object3dManager::GetInstance()->SetCamera(camera_.get());
	LineManager::GetInstance()->SetCamera(camera_.get());
	ParticleManager::GetInstance()->SetCamera(camera_.get());
	BulletTrailManager::GetInstance()->SetCamera(camera_.get());
	player_->SetGameCamera(camera_.get());
	playerWeaponManager_->SetGameCamera(camera_.get());
	playerUI_->SetGameCamera(camera_.get());
	enemyUI_->SetGameCamera(camera_.get());

	//ライトのセット
	Object3dManager::GetInstance()->SetSceneLight(sceneLight_.get());

	//その他インスタンスのセット
	startDirection_->SetMessageUI(messageUI_.get());
	endDirection_->SetTimeScaleManager(timeScaleManager_.get());
	endDirection_->SetPlayer(player_.get());
	endDirection_->SetEnemyManager(enemyManager_.get());
	player_->SetPlayerWeaponManager(playerWeaponManager_.get());
	player_->SetLevelLoader(levelLoader_.get());
	player_->SetMessageUI(messageUI_.get());
	enemyManager_->SetLevelLoader(levelLoader_.get());
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

	//必要なインスタンスの更新
	playerUI_->Update();
	player_->Update();
	enemyManager_->Update();
	camera_->Update();
}

void GamePlayScene::Finalize() {
	//デバッグモードを開始
	input_->SetIsMouseDisplay(true);
	input_->SetIsMouseFixed(false);
}

void GamePlayScene::Update() {
	//シーン共通の更新
	BaseScene::Update();

	//F1キーでマウスカーソルの表示する
	if (input_->TriggerKey(DIK_F1)) {
		if (isDebug_) {
			//デバッグモードを終了
			isDebug_ = false;
			input_->SetIsMouseDisplay(false);
			input_->SetIsMouseFixed(true);
		}
		else {
			//デバッグモードを開始
			isDebug_ = true;
			input_->SetIsMouseDisplay(true);
			input_->SetIsMouseFixed(false);
		}
	}

	//タイムスケールマネージャーの更新
	timeScaleManager_->Update();
	//ポーズシステムの更新
	pauseSystem_->Update();

	//スタート演出の更新
	startDirection_->Update();
	//エンド演出の更新
	endDirection_->Update();

	//タイムスケールマネージャーによる再生速度の管理
	if (!timeScaleManager_->GetIsPlay()) {
		return;
	}
	//ポーズ画面による再生の管理
	if (pauseSystem_->GetIsPause()) {
		return;
	}

	//メッセージUIの更新
	messageUI_->Update();

	//もしスタート演出中なら他の更新はしない
	if (startDirection_->GetIsStartDirection()) {
		return;
	}

	//インスタンスの更新
	levelLoader_->Update();
	playerUI_->Update();
	player_->Update();
	enemyManager_->Update();
	playerWeaponManager_->Update();
	enemyWeaponManager_->Update();
	itemManager_->Update();

	//カメラの更新(全インスタンスの処理が終わった後にやる)
	camera_->Update();

	//カメラの更新後の処理（スクリーン座標を参照したいインスタンスの更新）
	enemyUI_->Update();

}
void GamePlayScene::DebugWithImGui() {
	//ImGui
#ifdef _DEBUG
	//カメラのImGui
	camera_->DebugWithImGui();

	//レベルローダーのImGui
	levelLoader_->DebugWithImGui();
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
	//アイテムマネージャーのImGui
	itemManager_->DebugWithImGui();
	//メッセージUIのImGui
	messageUI_->DebugWithImGui();
	//ポーズシステムのImGui
	pauseSystem_->Debug();

	ImGui::Begin("距離");
	float distance = Vector3(player_->GetWorldTransform().GetTranslate() - enemyManager_->GetBosses()[0]->GetWorldTransform().GetTranslate()).Length();
	ImGui::Text("%f", distance);
	ImGui::End();


#endif // _DEBUG
}