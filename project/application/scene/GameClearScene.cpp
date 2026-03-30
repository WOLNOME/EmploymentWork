#include "GameClearScene.h"
#include "SceneManager.h"
#include "GameCamera.h"

using namespace Norm;

void GameClearScene::Initialize() {
	//シーン共通の初期化
	BaseScene::Initialize();

	//カメラマネージャーの生成と初期化
	cameraManager_ = std::make_unique<CameraManager>();
	cameraManager_->Initialize();

	//ゲームカメラの生成と初期化
	std::unique_ptr<GameCamera> gameCamera = std::make_unique<GameCamera>();
	gameCamera->Initialize();
	gameCamera->SetFarClip(2000.0f);
	//カメラマネージャーにゲームカメラをセット
	cameraManager_->RegistCamera("Game", std::move(gameCamera));
	//アクティブカメラをセット
	cameraManager_->SetActiveCamera("Game");

	//ライトの生成・初期化＆登録
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->Initialize();
	dirLight_->SetDirection({ 1.0f,-1.0f,1.0f });
	sceneLight_->SetLight(dirLight_.get());

	//インスタンスの生成
	skydome_ = std::make_unique<Skydome>();
	ground_ = std::make_unique<Ground>();
	gameClearSystem_ = std::make_unique<GameClearSystem>();

	//インスタンスの初期化
	skydome_->Initialize();
	ground_->Initialize();
	gameClearSystem_->Initialize();

	//その他インスタンスのセット
	gameClearSystem_->SetCameraManager(cameraManager_.get());

}

void GameClearScene::Finalize() {
}

void GameClearScene::Update() {
	//シーン共通の更新
	BaseScene::Update();

	//インスタンスの更新処理
	gameClearSystem_->Update();

	//カメラの更新(全インスタンスの処理が終わった後にやる)
	cameraManager_->Update();
}

void GameClearScene::DebugWithImGui() {
	//ImGui
#ifdef _DEBUG
	//ゲームクリアシステムのImGui
	gameClearSystem_->DebugWithImGui();
	//カメラのImGui
	cameraManager_->DebugWithImGui();

#endif // _DEBUG
}
