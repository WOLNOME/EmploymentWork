#include "TitleScene.h"
#include "SceneManager.h"

void TitleScene::Initialize() {
	//シーン共通の初期化
	BaseScene::Initialize();
	
	//インプットの初期化
	input_ = Input::GetInstance();

	//カメラの生成・初期化
	camera_ = std::make_unique<DevelopCamera>();
	camera_->Initialize();
	camera_->SetFarClip(2000.0f);
	camera_->worldTransform.SetRotate({ 0.15f,0.0f,0.0f });
	camera_->worldTransform.SetTranslate({ 0.0f,20.0f,-80.0f });

	//インスタンスの生成
	titleSystem_ = std::make_unique<TitleSystem>();

	//インスタンスの初期化
	titleSystem_->Initialize();

	//カメラのセット
	Object3dManager::GetInstance()->SetCamera(camera_.get());
	LineManager::GetInstance()->SetCamera(camera_.get());
	ParticleManager::GetInstance()->SetCamera(camera_.get());
	BulletTrailManager::GetInstance()->SetCamera(camera_.get());
	titleSystem_->SetCamera(camera_.get());

	//ライトのセット
	Object3dManager::GetInstance()->SetSceneLight(sceneLight_.get());

}

void TitleScene::Finalize() {
}

void TitleScene::Update() {
	//シーン共通の更新
	BaseScene::Update();

	titleSystem_->Update();

	//カメラの更新(全インスタンスの処理が終わった後にやる)
	camera_->Update();
}

void TitleScene::DebugWithImGui() {
	//ImGui
#ifdef _DEBUG
	//タイトルシステムのImGui
	titleSystem_->DebugWithImGui();
	//カメラのImGui
	camera_->DebugWithImGui();

#endif // _DEBUG
}
