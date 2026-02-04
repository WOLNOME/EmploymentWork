#include "GameClearSystem.h"
#include <WinApp.h>
#include <ImGuiManager.h>
#include <Object3dManager.h>
#include <TextTextureManager.h>
#include <TextureManager.h>
#include <SpriteManager.h>
#include <MyMath.h>
#include <cassert>

using namespace Norm;

void GameClearSystem::Initialize() {
	//インプット
	input_ = Input::GetInstance();
	//シーンマネージャー
	sceneManager_ = SceneManager::GetInstance();

	//ランダムエンジン
	std::random_device seed;
	std::mt19937 engine(seed());

	//戦車オブジェクト
	{
		uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("player.png");
		tank_ = std::make_unique<Object3d>();
		tank_->Initialize(ModelTag{}, Object3dManager::GetInstance()->GenerateName("tank"), "enemy");
		tank_->SetTexture(textureHandle);
		tank_->SetIsDisplay(true);
		tank_->worldTransform.SetTranslate({ 0.0f,3.0f,0.0f });
	}

	//クリアテキスト
	{
		std::wstring perfectText = L"GameClear!";
		TextParam textParam;
		textParam.color = { 1,1,1,1 };
		textParam.font = Font::UDDegitalNK_B;
		textParam.fontStyle = FontStyle::Normal;
		textParam.size = 90.0f;
		EdgeParam edgeParam;
		edgeParam.color = { 0,0,0,1 };
		edgeParam.isEdgeDisplay = true;
		edgeParam.slideRate = { 0,0 };
		edgeParam.width = 3.0f;

		//各文字ごとに登録
		for (int i = 0; i < perfectText.size(); i++) {
			textParam.text = perfectText.substr(i, 1);
			//テクスチャハンドルに登録
			Handle textHandle = TextTextureManager::GetInstance()->LoadTextTexture(textParam);
			TextTextureManager::GetInstance()->EditEdgeParam(textHandle, edgeParam);
			clearTextHandles_.emplace_back(textHandle);
			//スプライト
			std::unique_ptr<Sprite> textSprite = std::make_unique<Sprite>();
			textSprite->Initialize(TextTag{}, SpriteManager::GetInstance()->GenerateName("ClearTextUI"), Order::Front1);
			textSprite->SetPosition({ WinApp::kClientWidth / 2.0f - (perfectText.size() - 1) * 45.0f + i * 90.0f,WinApp::kClientHeight / 2.0f - 200.0f });
			textSprite->SetAnchorPoint({ 0.5f,0.5f });
			textSprite->SetTexture(textHandle);
			clearTextSprites_.emplace_back(std::move(textSprite));
		}
	}
	//クリアテキスト背景
	{
		std::uniform_real_distribution<float> dist(0.0f, 1.0f);
		Vector2 pos;
		//テクスチャハンドルに登録
		uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("white.png");
		//スプライト
		for (auto& sprite : clearTextSprites_) {
			std::unique_ptr<Sprite> bgSprite = std::make_unique<Sprite>();
			bgSprite->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("ClearTextBGUI"), Order::Front0, textureHandle);
			pos = sprite->GetPosition();
			pos.y += 5.0f;	//少し下にずらす
			bgSprite->SetPosition(pos);
			bgSprite->SetAnchorPoint({ 0.5f,0.5f });
			bgSprite->SetColor({ dist(engine),dist(engine),dist(engine),0.8f });
			bgSprite->SetSize({ 75.0f,80.0f });
			clearTextBack_.emplace_back(std::move(bgSprite));
		}
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
		titleTextSprite_->Initialize(TextTag{}, SpriteManager::GetInstance()->GenerateName("TitleStartUI"), Order::Front1);
		titleTextSprite_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
		titleTextSprite_->SetAnchorPoint({ 0.5f,0.5f });
		titleTextSprite_->SetTexture(titleTextHandle_);
	}

	//紙吹雪パーティクル
	{
		confettiParticle_ = std::make_unique<CombinedParticle>();
		confettiParticle_->Initialize(ParticleManager::GetInstance()->GenerateName("ConfettiParticle"), "Confetti");
		confettiParticle_->SetIsPlay(true);
		confettiParticle_->SetIsRepeat(true);
	}
}

void GameClearSystem::Update() {
	//ゲームカメラのチェック
	assert(gameCamera_ != nullptr && "ゲームカメラがセットされていません");

	//操作
	Operate();
	//UI演出
	DirectionUI();
	//カメラワーク
	CameraWork();
	//紙吹雪パーティクル更新
	ConfettiParticleUpdate();
}

void GameClearSystem::DebugWithImGui() {

}

void GameClearSystem::Operate() {
	//Spaceキーで開始
	if (input_->TriggerKey(DIK_SPACE)||input_->TriggerPadButton(GamePadButton::A)) {
		uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("shutter.png");
		sceneManager_->SetNextScene("Title", SceneTransitionAnimation::Type::SLIDEDOWN, SceneTransitionAnimation::Type::SLIDEUP, SceneTransitionAnimation::Option::SHAKE, 1.0f, textureHandle);
	}
}

void GameClearSystem::DirectionUI() {
	//必要変数の計算
	float allCharRotateEndTime;		//全文字回転終了時間
	allCharRotateEndTime = textDirectionParam_.allCharRotateStartTime + textDirectionParam_.charRotateDuration;
	std::vector<float> charRotateStartTime;		//各文字の回転開始時間
	std::vector<float> charRotateEndTime;		//各文字の回転終了時間
	for (int i = 0; i < clearTextSprites_.size(); i++) {
		float rate = float(i + 1.0f) / clearTextSprites_.size();
		charRotateStartTime.emplace_back(MyMath::Lerp(0.0f, textDirectionParam_.allCharRotateStartTime, MyMath::EaseInSine(rate)));
		charRotateEndTime.emplace_back(charRotateStartTime[i] + textDirectionParam_.charRotateDuration);
	}

	//タイマー更新
	textDirectionParam_.timer += kDeltaTime;
	//タイマーが最大値を超えたら
	if (textDirectionParam_.timer > allCharRotateEndTime) {
		//タイマーをリセット
		textDirectionParam_.timer = 0.0f;
		//スプライトの回転をリセット
		for (int i = 0; i < clearTextSprites_.size(); i++) {
			clearTextSprites_[i]->SetRotation(0.0f);
			clearTextBack_[i]->SetRotation(0.0f);
		}
	}

	//各文字の回転処理
	for (int i = 0; i < clearTextSprites_.size(); i++) {
		//回転しない時間なら
		if (textDirectionParam_.timer < charRotateStartTime[i] || textDirectionParam_.timer > charRotateEndTime[i]) {
			//回転を0にする
			clearTextSprites_[i]->SetRotation(0.0f);
			clearTextBack_[i]->SetRotation(0.0f);
			//次へ
			continue;
		}
		//回転中の処理
		float t = (textDirectionParam_.timer - charRotateStartTime[i]) / (charRotateEndTime[i] - charRotateStartTime[i]);
		float rotation = MyMath::Lerp(0.0f, 2.0f * pi, MyMath::EaseOutCubic(t));
		clearTextSprites_[i]->SetRotation(rotation);
		clearTextBack_[i]->SetRotation(rotation);
	}

}

void GameClearSystem::CameraWork() {
	if (!cameraWorkParam_.isEnd) {
		//タイマー更新
		cameraWorkParam_.timer += kDeltaTime;
		//タイマーが移動時間を超えたら
		if (cameraWorkParam_.timer > cameraWorkParam_.moveDuration) {
			cameraWorkParam_.timer = cameraWorkParam_.moveDuration;
			cameraWorkParam_.isEnd = true;
		}
		//カメラ位置更新
		float t = (float)cameraWorkParam_.timer / cameraWorkParam_.moveDuration;
		Vector3 cameraPos = MyMath::Lerp(cameraWorkParam_.startPos, cameraWorkParam_.endPos, MyMath::EaseOutQuad(t));
		gameCamera_->worldTransform.SetTranslate(cameraPos);
		//カメラの回転更新
		Vector3 targetPos = tank_->worldTransform.GetTranslate();
		targetPos.y += 2.5f;	//少し上を見る
		Vector3 cameraRotate = MyMath::DirectionToRotation(Vector3(targetPos - gameCamera_->worldTransform.GetTranslate()));
		gameCamera_->worldTransform.SetRotate(cameraRotate);
	}
}

void GameClearSystem::ConfettiParticleUpdate() {
	//パーティクルの座標をカメラの座標に合わせる
	TransformEuler transform = confettiParticle_->GetBaseTransform();
	transform.translate.y = gameCamera_->worldTransform.GetTranslate().y + 7.0f;
	confettiParticle_->SetBaseTransform(transform);
}
