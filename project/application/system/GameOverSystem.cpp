#include "GameOverSystem.h"
#include <WinApp.h>
#include <ImGuiManager.h>
#include <TextTextureManager.h>
#include <TextureManager.h>
#include <SpriteManager.h>
#include <MyMath.h>

using namespace Norm;

void GameOverSystem::Initialize() {
	//インプット
	input_ = Input::GetInstance();
	//シーンマネージャー
	sceneManager_ = SceneManager::GetInstance();

	//SEの初期化
	decideSE_ = std::make_unique<Audio>();
	decideSE_->Initialize("se/decide.mp3");

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
		gameOverTextSprite_->Initialize(TextTag{}, SpriteManager::GetInstance()->GenerateName("TitleStartUI"), Order::Front0);
		gameOverTextSprite_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f - 300.0f });
		gameOverTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
		gameOverTextSprite_->SetTexture(gameOverTextHandle_);
	}
	//タイトルテキスト
	{
		//テクスチャハンドルに登録
		titleTextHandle_ = TextureManager::GetInstance()->LoadTexture("toTitleUI.png");
		//スプライト
		titleTextSprite_ = std::make_unique<Sprite>();
		titleTextSprite_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("TitleStartUI"), Order::Front0, titleTextHandle_);
		titleTextSprite_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f + 50.0f });
		titleTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
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
	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerPadButton(GamePadButton::A)) {
		uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("shutter.png");
		if (sceneManager_->SetNextScene("Title", SceneTransitionAnimation::Type::SLIDEDOWN, SceneTransitionAnimation::Type::SLIDEUP, SceneTransitionAnimation::Option::SHAKE, 1.0f, textureHandle)) {
			//決定音を出す
			decideSE_->Play(false, 1.0f);
		}
	}
}

void GameOverSystem::DirectionUI() {
	//タイマー
	timer_ += kDeltaTime;
	if (timer_ >= kTime_) {
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
			Vector2 position = MyMath::Lerp(upPosition, downPosition, MyMath::EaseInSine(timer_ / kTime_));
			gameOverTextSprite_->SetPosition(position);
			//透明にしていく
			float alpha = MyMath::Lerp(1.0f, 0.0f, MyMath::EaseInSine(timer_ / kTime_));
			gameOverTextSprite_->SetColor({ 1,1,1,alpha });
		}
		//後周期
		else {
			//位置を上げる
			Vector2 position = MyMath::Lerp(downPosition, upPosition, MyMath::EaseOutSine(timer_ / kTime_));
			gameOverTextSprite_->SetPosition(position);
			//透明にしていく
			float alpha = MyMath::Lerp(0.0f, 1.0f, MyMath::EaseOutSine(timer_ / kTime_));
			gameOverTextSprite_->SetColor({ 1,1,1,alpha });
		}
	}
	//終了テキスト
	{
		//前周期
		if (isHalfPeriod_) {
			//透明にしていく
			float alpha = MyMath::Lerp(1.0f, 0.0f, MyMath::EaseInSine(timer_ / kTime_));
			titleTextSprite_->SetColor({ 1,1,1,alpha });
		}
		//後周期
		else {
			//透明にしていく
			float alpha = MyMath::Lerp(0.0f, 1.0f, MyMath::EaseOutSine(timer_ / kTime_));
			titleTextSprite_->SetColor({ 1,1,1,alpha });
		}
	}
}

