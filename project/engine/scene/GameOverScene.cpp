#include "GameOverScene.h"
#include "SceneManager.h"

void GameOverScene::Initialize() {
	//シーン共通の初期化
	BaseScene::Initialize();

	//インプットの初期化
	input_ = Input::GetInstance();

	//オブジェクトの生成・初期化
	{
		TextParam textParam;
		textParam.color = { 0.0f, 0.0f, 0.0f, 1.0f };
		textParam.size = 50;
		textParam.font = Font::UDDegitalNK_B;
		textParam.fontStyle = FontStyle::Normal;
		textParam.text = L"Game Over";
		thGameOverText_ = TextTextureManager::GetInstance()->LoadTextTexture(textParam);
		EdgeParam edgeParam;
		edgeParam.width = 2;
		edgeParam.isEdgeDisplay = 1;
		edgeParam.slideRate = { 0.0f, 0.0f };
		edgeParam.color = { 1, 0, 0, 1 };
		TextTextureManager::GetInstance()->EditEdgeParam(thGameOverText_, edgeParam);
		spriteGameOverText_ = std::make_unique<Sprite>();
		spriteGameOverText_->Initialize();
		spriteGameOverText_->SetPosition({ WinApp::kClientWidth / 2.0f, WinApp::kClientHeight / 2.0f });
		spriteGameOverText_->SetAnchorPoint({ 0.5f, 0.5f });
		
	}

}

void GameOverScene::Finalize() {
}

void GameOverScene::Update() {
	//シーン共通の更新
	BaseScene::Update();

	//スペースキーでgamesceneに移動
	if (input_->TriggerKey(DIK_SPACE)) {
		sceneManager_->SetNextScene("GamePlay");
	}

}

void GameOverScene::Draw() {
	//バックスプライト共通描画設定
	SpriteCommon::GetInstance()->SettingCommonDrawing();

	///------------------------------///
	///↓↓↓↓バックスプライト描画開始↓↓↓↓
	///------------------------------///



	///------------------------------///
	///↑↑↑↑バックスプライト描画終了↑↑↑↑
	///------------------------------///

	//3Dモデルの共通描画設定
	Object3dCommon::GetInstance()->SettingCommonDrawing();

	///------------------------------///
	///↓↓↓↓モデル描画開始↓↓↓↓
	///------------------------------///



	///------------------------------///
	///↑↑↑↑モデル描画終了↑↑↑↑
	///------------------------------///

	//フロントスプライト共通描画設定
	SpriteCommon::GetInstance()->SettingCommonDrawing();

	///------------------------------///
	///↓↓↓↓フロントスプライト描画開始↓↓↓↓
	///------------------------------///

	//スプライトの描画
	spriteGameOverText_->AdjustTextureSize(thGameOverText_);
	spriteGameOverText_->Draw(thGameOverText_);

	///------------------------------///
	///↑↑↑↑フロントスプライト描画終了↑↑↑↑
	///------------------------------///
}
