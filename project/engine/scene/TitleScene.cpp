#include "TitleScene.h"
#include "SceneManager.h"

void TitleScene::Initialize() {
	//シーン共通の初期化
	BaseScene::Initialize();

	input_ = Input::GetInstance();

	//生成と初期化
	textureHandleUI_ = TextureManager::GetInstance()->LoadTexture("UI_SPACE.png");
	spriteUI_ = std::make_unique<Sprite>();
	spriteUI_->Initialize(textureHandleUI_);

	//カメラの生成と初期化
	camera = std::make_unique<DevelopCamera>();
	camera->Initialize();
	camera->SetTranslate({ 0.0f,20.0f,-20.0f });
	camera->SetRotate({ 0.6f,0.0f,0.0f });
	//パーティクルマネージャーにカメラをセット
	ParticleManager::GetInstance()->SetCamera(camera.get());

	title_ = std::make_unique<TextWrite>();
	title_->Initialize("TITLE");
	title_->SetParam({ 320.0f,300.0f }, Font::OnionScript, 80.0f, { 1,1,0,1 });
	title_->SetEdgeParam({ 1,0,0,1 }, 5.0f, 0.0f, true);
	
}

void TitleScene::Finalize() {
}

void TitleScene::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		sceneManager_->SetNextScene("STAGESELECT");
	}

	//カメラの更新
	camera->Update();

#ifdef _DEBUG
	ImGui::Begin("scene");
	ImGui::Text("%s", "TITLE");
	ImGui::End();
	//タイトルテキスト用のImGui
	title_->DebugWithImGui();

#endif // _DEBUG

	spriteUI_->Update();

}

void TitleScene::Draw() {
	//3Dモデルの共通描画設定
	Object3dCommon::GetInstance()->SettingCommonDrawing();

	///------------------------------///
	///↓↓↓↓モデル描画開始↓↓↓↓
	///------------------------------///



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

	spriteUI_->Draw();

	///------------------------------///
	///↑↑↑↑スプライト描画終了↑↑↑↑
	///------------------------------///

}

void TitleScene::TextDraw() {
	///------------------------------///
	///↑↑↑↑テキスト描画終了↑↑↑↑
	///------------------------------///

	//タイトルテキスト
	title_->WriteText(L"ふえるぶつかり屋");


	///------------------------------///
	///↑↑↑↑テキスト描画終了↑↑↑↑
	///------------------------------///
}

