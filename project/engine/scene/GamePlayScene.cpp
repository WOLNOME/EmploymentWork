#include "GamePlayScene.h"
#include "SceneManager.h"

void GamePlayScene::Initialize() {
	//シーン共通の初期化
	BaseScene::Initialize();

	//インプットの初期化
	input_ = Input::GetInstance();

	//カメラの生成・初期化
	camera_ = std::make_unique<BaseCamera>();
	camera_->Initialize();
	camera_->SetFarClip(500.0f);
	camera_->worldTransform.rotate={ 0.15f,0.0f,0.0f };
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
	enemyManager_ = std::make_unique<EnemyManager>();
	//インスタンスの初期化
	skydome_->Initialize();
	ground_->Initialize();
	player_->Initialize();
	enemyManager_->Initialize();
	//カメラ、ライトのセット
	ParticleManager::GetInstance()->SetCamera(camera_.get());
	player_->SetCamera(camera_.get());
	enemyManager_->SetCamera(camera_.get());
	player_->SetSceneLight(sceneLight_.get());
	enemyManager_->SetLight(sceneLight_.get());
	//その他インスタンスのセット
	enemyManager_->SetPlayer(player_.get());

	//カメラのペアレントをプレイヤーのオブジェクトと結びつける
	player_->ParentForCamera();

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
	//インスタンスの更新
	skydome_->Update();
	ground_->Update();
	player_->Update();
	enemyManager_->Update();



	//カメラの更新(オブジェクト更新の後)
	camera_->Update();

	//ImGui
#ifdef _DEBUG
	//カメラのImGui
	camera_->DebugWithImGui();

	//プレイヤーのImGui
	player_->DebugWithImGui();
	//敵のImGui
	enemyManager_->DebugWithImGui();


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
	enemyManager_->Draw();

	///------------------------------///
	///↑↑↑↑モデル描画終了↑↑↑↑
	///------------------------------///

	//線描画共通描画設定
	LineDrawerCommon::GetInstance()->SettingCommonDrawing();

	///------------------------------///
	///↓↓↓↓線描画開始↓↓↓↓
	///------------------------------///

	//プレイヤーのライン描画
	player_->DrawLine();
	//敵のライン描画
	enemyManager_->DrawLine();


	///------------------------------///
	///↑↑↑↑線描画終了↑↑↑↑
	///------------------------------///

	//スプライトの共通描画設定
	SpriteCommon::GetInstance()->SettingCommonDrawing();

	///------------------------------///
	///↓↓↓↓スプライト描画開始↓↓↓↓
	///------------------------------///

	player_->DrawSprite();

	///------------------------------///
	///↑↑↑↑スプライト描画終了↑↑↑↑
	///------------------------------///
}

void GamePlayScene::TextDraw() {
}
