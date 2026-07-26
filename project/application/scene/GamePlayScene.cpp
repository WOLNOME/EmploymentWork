#include "GamePlayScene.h"
#include "SceneManager.h"
#include <TextureManager.h>
#include <GameCamera.h>

using namespace Norm;

void GamePlayScene::Initialize() {
	//シーン共通の初期化
	BaseScene::Initialize();

	//インプットの初期化
	input_ = Input::GetInstance();
	input_->SetIsMouseDisplay(false);
	input_->SetIsMouseFixed(true);

	//BGMの初期化
	bgm_ = std::make_unique<Audio>();
	bgm_->Initialize("bgm/gamePlay.mp3");
	bgm_->Play(true, 1.0f);

	//カメラマネージャーの生成・初期化
	cameraManager_ = std::make_unique<CameraManager>();
	cameraManager_->Initialize();

	//ゲーム用カメラを作ってマネージャーに登録する
	std::unique_ptr<GameCamera> gameCamera = std::make_unique<GameCamera>();
	gameCamera->Initialize();
	gameCamera->SetFarClip(2000.0f);
	cameraManager_->RegistCamera("Game", std::move(gameCamera));
	//アクティブカメラをゲーム用カメラにする
	cameraManager_->SetActiveCamera("Game");

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
	gamePlaySystem_ = std::make_unique<GamePlaySystem>();

	//インスタンスの初期化
	skydome_->Initialize();
	ground_->Initialize();
	levelLoader_->Initialize("Resources/levelData/levelData");
	player_->Initialize();
	playerWeaponManager_->Initialize();
	enemyManager_->Initialize();
	enemyWeaponManager_->Initialize();
	itemManager_->Initialize();
	playerUI_->Initialize();
	enemyUI_->Initialize();
	messageUI_->Initialize();
	pauseSystem_->Initialize();
	gamePlaySystem_->Initialize();

	//カメラマネージャーのセット
	player_->SetCameraManager(cameraManager_.get());
	playerWeaponManager_->SetCameraManager(cameraManager_.get());
	playerUI_->SetCameraManager(cameraManager_.get());
	enemyUI_->SetCameraManager(cameraManager_.get());
	enemyManager_->SetCameraManager(cameraManager_.get());
	enemyWeaponManager_->SetCameraManager(cameraManager_.get());

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
	gamePlaySystem_->SetPlayer(player_.get());

	//パラメーターのセット
	player_->SetMoveLimitDistance(995.0f);

	//必要なインスタンスの更新
	player_->Update();
	enemyManager_->Update();
	playerUI_->Update();
	cameraManager_->Update();
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
	gamePlaySystem_->Update();

	//カメラの更新(全インスタンスの処理が終わった後にやる)
	cameraManager_->Update();

	//カメラの更新後の処理（スクリーン座標を参照したいインスタンスの更新）
	enemyUI_->Update();


	//ボス出現演出
	if (!isBossAppear_) {
		//もし鍵を2つあつめたら
		if (player_->GetKeyNum() == 2) {
			isBossAppear_ = true;
			uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("black.png");
			SceneManager::GetInstance()->SetNextScene("DIR_BossAppear",
				SceneTransitionAnimation::Type::FADE,
				SceneTransitionAnimation::Type::FADE,
				SceneTransitionAnimation::Option::NONE,
				0.5f, textureHandle, TransitionMode::Temporary
			);

			//演出通知
			player_->GetMoveSE()->Stop();

		}
	}

}
void GamePlayScene::DebugWithImGui() {
	//ImGui
#ifdef _DEBUG
	//カメラのImGui
	cameraManager_->DebugWithImGui();

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
	//ゲームプレイシステムのImGui
	gamePlaySystem_->DebugWithImGui();

#endif // _DEBUG
}

void GamePlayScene::OnResume() {
	//基底クラスの復帰時処理
	BaseScene::OnResume();

	//アクティブカメラをゲーム専用カメラにする
	cameraManager_->SetActiveCamera("Game");

	//封印オブジェクトを消す
	levelLoader_->GetSealedBoxData()->GetObject3d()->SetIsDisplay(false);
	for (auto& collider : levelLoader_->GetSealedBoxData()->GetColliders()) {
		collider.second->SetCollisionAttribute(CollisionAttribute::Nothingness);
	}

	//ボスを出現させる
	enemyManager_->BossSpawn({ 0,0,0 }, { 0,pi,0 });

	//演出シーンからの復帰時処理
	player_->GetMoveSE()->Play(true,0.0f);
}
