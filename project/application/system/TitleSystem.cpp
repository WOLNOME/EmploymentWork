#include "TitleSystem.h"
#include <WinApp.h>
#include <ImGuiManager.h>
#include <TextTextureManager.h>
#include <SpriteManager.h>
#include <MyMath.h>

void TitleSystem::Initialize() {
	//インプット
	input_ = Input::GetInstance();
	//シーンマネージャー
	sceneManager_ = SceneManager::GetInstance();

	//メンバ変数の初期化
	menu_ = Menu::Start;

	//スタートテキスト
	{
		TextParam textParam;
		textParam.color = { 1,1,1,1 };
		textParam.font = Font::UDDegitalNK_B;
		textParam.fontStyle = FontStyle::Normal;
		textParam.size = 48.0f;
		textParam.text = L"[SPACE]ではじめる";
		EdgeParam edgeParam;
		edgeParam.color = { 0,0,0,1 };
		edgeParam.isEdgeDisplay = true;
		edgeParam.slideRate = { 0,0 };
		edgeParam.width = 3.0f;
		//テクスチャハンドルに登録
		startTextHandle_ = TextTextureManager::GetInstance()->LoadTextTexture(textParam);
		TextTextureManager::GetInstance()->EditEdgeParam(startTextHandle_, edgeParam);
		//スプライト
		startTextSprite_ = std::make_unique<Sprite>();
		startTextSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("TitleStartUI"), Sprite::Order::Front0);
		startTextSprite_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
		startTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
		startTextSprite_->SetTexture(startTextHandle_);
	}
	//終了テキスト
	{
		TextParam textParam;
		textParam.color = { 1,1,1,1 };
		textParam.font = Font::UDDegitalNK_B;
		textParam.fontStyle = FontStyle::Normal;
		textParam.size = 24.0f;
		textParam.text = L"[ESCAPE]でおわる";
		EdgeParam edgeParam;
		edgeParam.color = { 0,0,0,1 };
		edgeParam.isEdgeDisplay = true;
		edgeParam.slideRate = { 0,0 };
		edgeParam.width = 2.0f;
		//テクスチャハンドルに登録
		exitTextHandle_ = TextTextureManager::GetInstance()->LoadTextTexture(textParam);
		TextTextureManager::GetInstance()->EditEdgeParam(exitTextHandle_, edgeParam);
		//スプライト
		exitTextSprite_ = std::make_unique<Sprite>();
		exitTextSprite_->Initialize(SpriteManager::GetInstance()->GenerateName("TitleStartUI"), Sprite::Order::Front0);
		exitTextSprite_->SetPosition({ 120.0f,700.0f });
		exitTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
		exitTextSprite_->SetTexture(exitTextHandle_);
	}


}

void TitleSystem::Update() {
	//操作
	Operate();
	//UI演出
	DirectionUI();
}

void TitleSystem::DebugWithImGui() {

}

void TitleSystem::Operate() {
	//Spaceキーで開始
	if (input_->TriggerKey(DIK_SPACE)) {
		sceneManager_->SetNextScene("GamePlay");
	}

	//Escapeキーで終了
	if (input_->TriggerKey(DIK_ESCAPE)) {
	}
}

void TitleSystem::DirectionUI() {
	//タイマー
	timer_ += kDeltaTime;
	if (timer_ >= time_) {
		isHalfPeriod_ = !isHalfPeriod_;
		timer_ = 0.0f;
	}

	//スタートテキスト
	{
		//周期別の処理
		Vector2 upPosition = { WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f };
		Vector2 downPosition = { WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f + 30.0f };
		//前周期
		if (isHalfPeriod_) {
			//位置を下げる
			Vector2 position = MyMath::Lerp(upPosition, downPosition, MyMath::EaseInSine(timer_ / time_));
			startTextSprite_->SetPosition(position);
			//透明にしていく
			float alpha = MyMath::Lerp(1.0f, 0.0f, MyMath::EaseInSine(timer_ / time_));
			startTextSprite_->SetColor({ 1,1,1,alpha });
		}
		//後周期
		else {
			//位置を上げる
			Vector2 position = MyMath::Lerp(downPosition, upPosition, MyMath::EaseOutSine(timer_ / time_));
			startTextSprite_->SetPosition(position);
			//透明にしていく
			float alpha = MyMath::Lerp(0.0f, 1.0f, MyMath::EaseOutSine(timer_ / time_));
			startTextSprite_->SetColor({ 1,1,1,alpha });
		}
	}
	//終了テキスト
	{
		//前周期
		if (isHalfPeriod_) {
			//透明にしていく
			float alpha = MyMath::Lerp(1.0f, 0.0f, MyMath::EaseInSine(timer_ / time_));
			exitTextSprite_->SetColor({ 1,1,1,alpha });
		}
		//後周期
		else {
			//透明にしていく
			float alpha = MyMath::Lerp(0.0f, 1.0f, MyMath::EaseOutSine(timer_ / time_));
			exitTextSprite_->SetColor({ 1,1,1,alpha });
		}
	}
}
