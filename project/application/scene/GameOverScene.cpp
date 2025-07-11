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
		textParam.text = L"Game Over...";
		thGameOverText_ = TextTextureManager::GetInstance()->LoadTextTexture(textParam);
		EdgeParam edgeParam;
		edgeParam.width = 2;
		edgeParam.isEdgeDisplay = 1;
		edgeParam.slideRate = { 0.0f, 0.0f };
		edgeParam.color = { 1, 0, 0, 1 };
		TextTextureManager::GetInstance()->EditEdgeParam(thGameOverText_, edgeParam);
		spriteGameOverText_ = std::make_unique<Sprite>();
		spriteGameOverText_->Initialize(SpriteManager::GetInstance()->GenerateName("GameOverText"), Sprite::Order::Front0, thGameOverText_);
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

void GameOverScene::DebugWithImGui() {
#ifdef _DEBUG
	SpriteManager::GetInstance()->DebugWithImGui();
#endif // _DEBUG
}
