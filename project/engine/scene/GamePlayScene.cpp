#include "GamePlayScene.h"

void GamePlayScene::Initialize() {
	//シーン共通の初期化
	BaseScene::Initialize();

	//カメラの生成・初期化
	camera_ = std::make_unique<DevelopCamera>();
	camera_->Initialize();
	camera_->SetFarClip(500.0f);
	camera_->SetRotate({ 0.15f,0.0f,0.0f });
	camera_->SetTranslate({ 0.0f,20.0f,-80.0f });

	//ライトの生成・初期化＆登録
	dirLight_ = std::make_unique<DirectionalLight>();
	dirLight_->Initialize();
	dirLight_->direction_ = { 1.0f,-1.0f,1.0f };
	sceneLight_->SetLight(dirLight_.get());

	//インスタンスの生成
	skydome_ = std::make_unique<Skydome>();
	ground_ = std::make_unique<Ground>();
	player_ = std::make_unique<Player>();
	//インスタンスの初期化
	skydome_->Initialize();
	ground_->Initialize();
	player_->Initialize();
	//カメラのセット
	player_->SetCamera(camera_.get());


}

void GamePlayScene::Finalize() {
}

void GamePlayScene::Update() {
	//シーン共通の更新
	BaseScene::Update();

	//カメラの更新
	camera_->Update();

	//インスタンスの更新
	skydome_->Update();
	ground_->Update();
	player_->Update();

	//ImGui
#ifdef _DEBUG
	//FPS表示
	//BaseScene::ShowFPS();
	//カメラのImGui
	camera_->DebugWithImGui();


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
	player_->Draw(*camera_.get(),sceneLight_.get());

	///------------------------------///
	///↑↑↑↑モデル描画終了↑↑↑↑
	///------------------------------///


	//線描画共通描画設定
	LineDrawerCommon::GetInstance()->SettingCommonDrawing();

	///------------------------------///
	///↓↓↓↓線描画開始↓↓↓↓
	///------------------------------///



	///------------------------------///
	///↑↑↑↑線描画終了↑↑↑↑
	///------------------------------///

	//スプライトの共通描画設定
	SpriteCommon::GetInstance()->SettingCommonDrawing();

	///------------------------------///
	///↓↓↓↓スプライト描画開始↓↓↓↓
	///------------------------------///



	///------------------------------///
	///↑↑↑↑スプライト描画終了↑↑↑↑
	///------------------------------///
}

void GamePlayScene::TextDraw() {
}
