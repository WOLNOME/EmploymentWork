#include "TitleSystem.h"
#include <WinApp.h>
#include <ImGuiManager.h>
#include <TextTextureManager.h>
#include <Object3dManager.h>
#include <SpriteManager.h>
#include <MyMath.h>
#include <cassert>

void TitleSystem::Initialize() {
	//インプット
	input_ = Input::GetInstance();
	//シーンマネージャー
	sceneManager_ = SceneManager::GetInstance();

	//メンバ変数の初期化
	menu_ = Menu::kStart;

	//タイトルテキスト
	{
		TextParam textParam;
		textParam.color = { 1,1,1,1 };
		textParam.font = Font::UDDegitalNK_B;
		textParam.fontStyle = FontStyle::Normal;
		textParam.size = 96.0f;
		textParam.text = L"タンク決戦";
		EdgeParam edgeParam;
		edgeParam.color = { 0,0,0,1 };
		edgeParam.isEdgeDisplay = true;
		edgeParam.slideRate = { 0,0 };
		edgeParam.width = 5.0f;
		//テクスチャハンドルに登録
		titleTextHandle_ = TextTextureManager::GetInstance()->LoadTextTexture(textParam);
		TextTextureManager::GetInstance()->EditEdgeParam(titleTextHandle_, edgeParam);
		//スプライト
		titleTextSprite_ = std::make_unique<Sprite>();
		titleTextSprite_->Initialize(TextTag{}, SpriteManager::GetInstance()->GenerateName("TitleUI"), Order::Front0);
		titleTextSprite_->SetPosition({ WinApp::kClientWidth / 2.0f,150.0f });
		titleTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
		titleTextSprite_->SetTexture(titleTextHandle_);
	}

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
		startTextSprite_->Initialize(TextTag{}, SpriteManager::GetInstance()->GenerateName("TitleStartUI"), Order::Front0);
		startTextSprite_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
		startTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
		startTextSprite_->SetTexture(startTextHandle_);
	}

	//オブジェクト
	{
		//ガレージ
		garage_ = std::make_unique<Object3d>();
		garage_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("garage"), "garage");

		//戦車
		for (int i = 0; i < 5; i++) {
			uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("player.png");
			auto tank = std::make_unique<Object3d>();
			tank->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("tank"), "enemy");
			tank->SetTexture(textureHandle);
			tank->worldTransform.translate = { -60.0f + (30.0f * i),3.1f,0.0f };
			tanks_.push_back(std::move(tank));
		}

	}

}

void TitleSystem::Update() {
	//操作
	Operate();
	//UI演出
	DirectionUI();
	//カメラ操作
	OperateCamera();
}

void TitleSystem::DebugWithImGui() {

}

void TitleSystem::Operate() {
	//Spaceキーで開始
	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerPadButton(GamePadButton::A)) {
		sceneManager_->SetNextScene("GamePlay", SceneTransitionAnimation::Type::SLIDEDOWN, SceneTransitionAnimation::Type::SLIDEUP, SceneTransitionAnimation::Option::SHAKE, 1.0f, TextureManager::GetInstance()->LoadTexture("shutter.png"));
	}

	//Escapeキーで終了
	if (input_->TriggerKey(DIK_ESCAPE)) {
	}
}

void TitleSystem::DirectionUI() {
	//タイマー
	timer_ += kDeltaTime;
	if (timer_ >= kTime_) {
		isHalfPeriod_ = !isHalfPeriod_;
		timer_ = 0.0f;
	}

	//スタートテキスト
	{
		//周期別の処理
		Vector2 upPosition = { WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f + 40.0f };
		Vector2 downPosition = { WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f + 70.0f };
		//前周期
		if (isHalfPeriod_) {
			//位置を下げる
			Vector2 position = MyMath::Lerp(upPosition, downPosition, MyMath::EaseInSine(timer_ / kTime_));
			startTextSprite_->SetPosition(position);
			//透明にしていく
			float alpha = MyMath::Lerp(1.0f, 0.0f, MyMath::EaseInSine(timer_ / kTime_));
			startTextSprite_->SetColor({ 1,1,1,alpha });
		}
		//後周期
		else {
			//位置を上げる
			Vector2 position = MyMath::Lerp(downPosition, upPosition, MyMath::EaseOutSine(timer_ / kTime_));
			startTextSprite_->SetPosition(position);
			//透明にしていく
			float alpha = MyMath::Lerp(0.0f, 1.0f, MyMath::EaseOutSine(timer_ / kTime_));
			startTextSprite_->SetColor({ 1,1,1,alpha });
		}
	}
}

void TitleSystem::OperateCamera() {
	//カメラのセット確認
	assert(camera_ != nullptr && "カメラがセットされていません");

	//移動
	{
		//タイマー
		cameraTimer_ += kDeltaTime;
		if (cameraTimer_ >= kCameraMoveTime_) {
			isHalfPeriodCamera_ = !isHalfPeriodCamera_;
			cameraTimer_ = 0.0f;
		}
		//前周期
		if (!isHalfPeriodCamera_) {
			Vector3 position = MyMath::Lerp(cameraStartPos_, cameraEndPos_, MyMath::EaseInOutSine(cameraTimer_ / kCameraMoveTime_));
			camera_->worldTransform.translate = position;
		}
		//後周期
		else {
			Vector3 position = MyMath::Lerp(cameraEndPos_, cameraStartPos_, MyMath::EaseInOutSine(cameraTimer_ / kCameraMoveTime_));
			camera_->worldTransform.translate = position;
		}
	}

	//回転
	{
		// カメラからターゲットへの方向ベクトル
		Vector3 diff = cameraTargetPos_ - camera_->worldTransform.translate;
		diff.Normalize();

		// Y軸（水平面）上の角度（ヨー / azimuth）
		float azimuth = std::atan2(diff.x, diff.z);

		// 上下角（ピッチ / elevation）
		float elevation = std::atan2(-diff.y, std::sqrt(diff.x * diff.x + diff.z * diff.z));

		// 回転をセット
		camera_->worldTransform.rotate = { elevation, azimuth, 0.0f };
	}
}