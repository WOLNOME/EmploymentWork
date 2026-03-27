#include "TutorialScene.h"
#include "SceneManager.h"
#include "GameCamera.h"
#include "DevelopCamera.h"

using namespace Norm;

void TutorialScene::Initialize() {
	//シーン共通の初期化
	BaseScene::Initialize();

	//カメラマネージャーの生成と初期化
	cameraManager_ = std::make_unique<CameraManager>();
	cameraManager_->Initialize();

	//ゲームカメラの生成と初期化
	std::unique_ptr<GameCamera> gameCamera = std::make_unique<GameCamera>();
	gameCamera->Initialize();
	gameCamera->SetFarClip(2000.0f);
	cameraManager_->RegistCamera("Game", std::move(gameCamera));
	//開発カメラの生成と初期化
	std::unique_ptr<DevelopCamera> developCamera = std::make_unique<DevelopCamera>();
	developCamera->Initialize();
	developCamera->SetFarClip(2000.0f);
	cameraManager_->RegistCamera("Develop", std::move(developCamera));
	//アクティブカメラのセット
	cameraManager_->SetActiveCamera("Game");

	//ライトの生成・初期化＆登録
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->Initialize();
	dirLight_->SetDirection({ 1.0f,-1.0f,1.0f });
	sceneLight_->SetLight(dirLight_.get());

	//インスタンスの生成
	skydome_ = std::make_unique<Skydome>();
	ground_ = std::make_unique<Ground>();
	system_ = std::make_unique<TutorialSystem>();

	//インスタンスの初期化
	skydome_->Initialize();
	ground_->Initialize();
	system_->Initialize();

	//その他インスタンスのセット
	system_->SetCameraManager(cameraManager_.get());
}

void TutorialScene::Finalize() {
}

void TutorialScene::Update() {
	//シーン共通の更新
	BaseScene::Update();

	//F1キーでマウスカーソルの表示する
	Input* input = Input::GetInstance();
	if (input->TriggerKey(DIK_F1)) {
		if (isDebug_) {
			//デバッグモードを終了
			isDebug_ = false;
			input->SetIsMouseDisplay(false);
			input->SetIsMouseFixed(true);
		}
		else {
			//デバッグモードを開始
			isDebug_ = true;
			input->SetIsMouseDisplay(true);
			input->SetIsMouseFixed(false);
		}
	}

	//インスタンスの更新
	system_->Update();

	//カメラの更新(全インスタンスの処理が終わった後にやる)
	cameraManager_->Update();
}

void TutorialScene::DebugWithImGui() {
	//ImGui
#ifdef _DEBUG
	//カメラのImGui
	cameraManager_->DebugWithImGui();
	//システム
	system_->DebugWithImGui();

#endif // _DEBUG
}
