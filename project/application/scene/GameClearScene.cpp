#include "GameClearScene.h"
#include "SceneManager.h"

void GameClearScene::Initialize() {
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

	//インスタンスの生成
	skydome_ = std::make_unique<Skydome>();
	ground_ = std::make_unique<Ground>();
	gameClearSystem_ = std::make_unique<GameClearSystem>();

	//インスタンスの初期化
	skydome_->Initialize();
	ground_->Initialize();
	gameClearSystem_->Initialize();

	//カメラのセット
	Object3dManager::GetInstance()->SetCamera(camera_.get());
	LineManager::GetInstance()->SetCamera(camera_.get());
	ParticleManager::GetInstance()->SetCamera(camera_.get());
	BulletTrailManager::GetInstance()->SetCamera(camera_.get());

	//ライトのセット
	Object3dManager::GetInstance()->SetSceneLight(sceneLight_.get());

}

void GameClearScene::Finalize() {
}

void GameClearScene::Update() {
	//シーン共通の更新
	BaseScene::Update();

	gameClearSystem_->Update();

	//カメラの更新(全インスタンスの処理が終わった後にやる)
	camera_->Update();
}

void GameClearScene::DebugWithImGui() {
	//ImGui
#ifdef _DEBUG
	//ゲームクリアシステムのImGui
	gameClearSystem_->DebugWithImGui();
	//カメラのImGui
	camera_->DebugWithImGui();

#endif // _DEBUG
}
