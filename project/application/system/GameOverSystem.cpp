#include "GameOverSystem.h"
#include <WinApp.h>
#include <ImGuiManager.h>
#include <TextTextureManager.h>
#include <SpriteManager.h>
#include <MyMath.h>

void GameOverSystem::Initialize() {
	//インプット
	input_ = Input::GetInstance();
	//シーンマネージャー
	sceneManager_ = SceneManager::GetInstance();

	//ゲームオーバーテキスト
	{
		TextParam textParam;
		textParam.color = { 1,1,0,1 };
		textParam.font = Font::UDDegitalNK_B;
		textParam.fontStyle = FontStyle::Normal;
		textParam.size = 60.0f;
		textParam.text = L"ゲームオーバー...";
		EdgeParam edgeParam;
		edgeParam.color = { 1,0,0,1 };
		edgeParam.isEdgeDisplay = true;
		edgeParam.slideRate = { 0,0 };
		edgeParam.width = 3.0f;
		//テクスチャハンドルに登録
		gameOverTextHandle_ = TextTextureManager::GetInstance()->LoadTextTexture(textParam);
		TextTextureManager::GetInstance()->EditEdgeParam(gameOverTextHandle_, edgeParam);
		//スプライト
		gameOverTextSprite_ = std::make_unique<Sprite>();
		gameOverTextSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("TitleStartUI"), Sprite::Order::Front0);
		gameOverTextSprite_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f - 300.0f });
		gameOverTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
		gameOverTextSprite_->SetTexture(gameOverTextHandle_);
	}
	//タイトルテキスト
	{
		TextParam textParam;
		textParam.color = { 1,1,1,1 };
		textParam.font = Font::UDDegitalNK_B;
		textParam.fontStyle = FontStyle::Normal;
		textParam.size = 24.0f;
		textParam.text = L"[SPACE]でタイトルに戻る";
		EdgeParam edgeParam;
		edgeParam.color = { 0,0,0,1 };
		edgeParam.isEdgeDisplay = true;
		edgeParam.slideRate = { 0,0 };
		edgeParam.width = 2.0f;
		//テクスチャハンドルに登録
		titleTextHandle_ = TextTextureManager::GetInstance()->LoadTextTexture(textParam);
		TextTextureManager::GetInstance()->EditEdgeParam(titleTextHandle_, edgeParam);
		//スプライト
		titleTextSprite_ = std::make_unique<Sprite>();
		titleTextSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("TitleStartUI"), Sprite::Order::Front0);
		titleTextSprite_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f + 50.0f });
		titleTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
		titleTextSprite_->SetTexture(titleTextHandle_);
	}


}

void GameOverSystem::Update() {
	//操作
	Operate();
	//UI演出
	DirectionUI();
}

void GameOverSystem::DebugWithImGui() {

}

void GameOverSystem::Operate() {
	//Spaceキーで開始
	if (input_->TriggerKey(DIK_SPACE)) {
		sceneManager_->SetNextScene("Title");
	}
}

void GameOverSystem::DirectionUI() {
	//タイマー
	timer_ += kDeltaTime;
	if (timer_ >= time_) {
		isHalfPeriod_ = !isHalfPeriod_;
		timer_ = 0.0f;
	}

	//スタートテキスト
	{
		//周期別の処理
		Vector2 upPosition = { WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f - 250.0f };
		Vector2 downPosition = { WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f - 200.0f };
		//前周期
		if (isHalfPeriod_) {
			//位置を下げる
			Vector2 position = MyMath::Lerp(upPosition, downPosition, MyMath::EaseInSine(timer_ / time_));
			gameOverTextSprite_->SetPosition(position);
			//透明にしていく
			float alpha = MyMath::Lerp(1.0f, 0.0f, MyMath::EaseInSine(timer_ / time_));
			gameOverTextSprite_->SetColor({ 1,1,1,alpha });
		}
		//後周期
		else {
			//位置を上げる
			Vector2 position = MyMath::Lerp(downPosition, upPosition, MyMath::EaseOutSine(timer_ / time_));
			gameOverTextSprite_->SetPosition(position);
			//透明にしていく
			float alpha = MyMath::Lerp(0.0f, 1.0f, MyMath::EaseOutSine(timer_ / time_));
			gameOverTextSprite_->SetColor({ 1,1,1,alpha });
		}
	}
	//終了テキスト
	{
		//前周期
		if (isHalfPeriod_) {
			//透明にしていく
			float alpha = MyMath::Lerp(1.0f, 0.0f, MyMath::EaseInSine(timer_ / time_));
			titleTextSprite_->SetColor({ 1,1,1,alpha });
		}
		//後周期
		else {
			//透明にしていく
			float alpha = MyMath::Lerp(0.0f, 1.0f, MyMath::EaseOutSine(timer_ / time_));
			titleTextSprite_->SetColor({ 1,1,1,alpha });
		}
	}
}

