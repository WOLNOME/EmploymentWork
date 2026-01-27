#include "PauseSystem.h"
#include <Input.h>
#include <SceneManager.h>
#include <TextureManager.h>
#include <SpriteManager.h>
#include <TextTextureManager.h>
#include <WinApp.h>

PauseSystem::PauseSystem() {
}

PauseSystem::~PauseSystem() {
}

void PauseSystem::Initialize() {
	//インスタンスの初期化
	input_ = Input::GetInstance();

	//背景マスクの生成・初期化
	{
		//テクスチャハンドル
		maskTextureHandle_ = TextureManager::GetInstance()->LoadTexture("black.png");
		//スプライト
		maskSprite_ = std::make_unique<Sprite>();
		maskSprite_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("mask"), Order::Front4, maskTextureHandle_);
		maskSprite_->SetColor({ 1,1,1,0.66f });
		maskSprite_->SetIsDisplay(false);
		maskSprite_->SetAnchorPoint({ 0.5f,0.5f });
		maskSprite_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
	}
	//操作ガイドの生成・初期化
	{
		//テクスチャハンドル
		guideTextureHandle_ = TextureManager::GetInstance()->LoadTexture("operationGuide.png");
		//スプライト
		guideSprite_ = std::make_unique<Sprite>();
		guideSprite_->Initialize(SpriteTag{}, SpriteManager::GetInstance()->GenerateName("operationGuide"), Order::Front5, guideTextureHandle_);
		guideSprite_->SetIsDisplay(false);
		guideSprite_->SetAnchorPoint({ 0.5f,0.5f });
		guideSprite_->SetPosition({ WinApp::kClientWidth / 2.0f,WinApp::kClientHeight / 2.0f });
		guideSprite_->SetSize({ 1280.0f,720.0f });
	}
	//メニューの文字の生成・初期化
	{
		//テキストパラメーター
		TextParam textParam;
		textParam.text = L"";
		textParam.font = Font::UDDegitalNK_R;
		textParam.fontStyle = FontStyle::Normal;
		textParam.size = 32.0f;
		textParam.color = { 1,1,1,1 };
		//エッジパラメーター
		EdgeParam edgeParam;
		edgeParam.width = 3;
		edgeParam.isEdgeDisplay = 1;
		edgeParam.slideRate = { 0,0 };
		edgeParam.color = { 0,0,0,1 };

		//全要素を走査
		for (int i = 0; i < kMaxStringNum_; i++) {
			//テクスチャハンドル
			stringMenuTextureHandle_[i] = TextTextureManager::GetInstance()->LoadTextTexture(textParam);
			TextTextureManager::GetInstance()->EditEdgeParam(stringMenuTextureHandle_[i], edgeParam);
			//スプライト
			stringMenuSprite_[i] = std::make_unique<Sprite>();
			stringMenuSprite_[i]->Initialize(TextTag{}, SpriteManager::GetInstance()->GenerateName("stringMenu"), Order::Front5);
			stringMenuSprite_[i]->SetIsDisplay(false);
			stringMenuSprite_[i]->SetAnchorPoint({ 0.5f,0.5f });
			stringMenuSprite_[i]->SetTexture(stringMenuTextureHandle_[i]);

			//各要素ごとの処理
			switch (i) {
			case 0: {
				//ポーズ中テキスト
				TextTextureManager::GetInstance()->EditTextString(stringMenuTextureHandle_[i], L"ポーズ中");
				//サイズを1.5倍にする
				TextTextureManager::GetInstance()->EditTextSize(stringMenuTextureHandle_[i], 48.0f);
				//場所を決める
				stringMenuSprite_[i]->SetPosition({ WinApp::kClientWidth / 2.0f,170.0f });

				break;
			}
			case 1: {
				//続けるテキスト
				TextTextureManager::GetInstance()->EditTextString(stringMenuTextureHandle_[i], L"続ける");
				//場所を決める
				stringMenuSprite_[i]->SetPosition({ WinApp::kClientWidth / 2.0f,350.0f });
				break;
			}
			case 2: {
				//操作ガイドテキスト
				TextTextureManager::GetInstance()->EditTextString(stringMenuTextureHandle_[i], L"操作ガイド");
				//場所を決める
				stringMenuSprite_[i]->SetPosition({ WinApp::kClientWidth / 2.0f,470.0f });

				break;
			}
			case 3: {
				//タイトルへテキスト
				TextTextureManager::GetInstance()->EditTextString(stringMenuTextureHandle_[i], L"タイトルに戻る");
				//場所を決める
				stringMenuSprite_[i]->SetPosition({ WinApp::kClientWidth / 2.0f,590.0f });
				break;
			}
			default:
				break;
			}
		}
	}

	//メンバ変数の初期化
	selectMenu_ = Menu::kContinue;
	isPause_ = false;
	isOperationGuideDisplay_ = false;
}

void PauseSystem::Update() {
	//ポーズ切り替え処理
	if (input_->TriggerKey(DIK_ESCAPE)) {
		isPause_ = !isPause_;
		isOperationGuideDisplay_ = false;
	}

	//ポーズ中の処理
	if (isPause_) {
		//メニューを表示
		maskSprite_->SetIsDisplay(true);
		for (int i = 0; i < kMaxStringNum_; i++) {
			stringMenuSprite_[i]->SetIsDisplay(true);
		}

		//メニュー決定＆切り替え
		switch (selectMenu_) {
		case PauseSystem::Menu::kContinue: {
			//決定
			if (input_->TriggerKey(DIK_SPACE)) {
				//ポーズ画面を終了
				isPause_ = false;
			}
			//下
			if (input_->TriggerKey(DIK_S)) {
				//メニューを操作ガイドに切り替え
				selectMenu_ = Menu::kOperationGuide;
			}

			//色を変える
			stringMenuSprite_[1]->SetColor({ 0.929f,0.592f,0.255f,1.0f });
			stringMenuSprite_[2]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
			stringMenuSprite_[3]->SetColor({ 1.0f,1.0f,1.0f,1.0f });


			break;
		}
		case PauseSystem::Menu::kOperationGuide: {
			//決定
			if (input_->TriggerKey(DIK_SPACE)) {
				//操作ガイドを表示
				isOperationGuideDisplay_ = true;
			}
			//上
			if (input_->TriggerKey(DIK_W)) {
				//メニューを続行に切り替え
				selectMenu_ = Menu::kContinue;
			}
			//下
			if (input_->TriggerKey(DIK_S)) {
				//メニューをタイトルに切り替え
				selectMenu_ = Menu::kTItle;
			}

			//色を変える
			stringMenuSprite_[1]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
			stringMenuSprite_[2]->SetColor({ 0.929f,0.592f,0.255f,1.0f });
			stringMenuSprite_[3]->SetColor({ 1.0f,1.0f,1.0f,1.0f });


			break;
		}
		case PauseSystem::Menu::kTItle: {
			//決定
			if (input_->TriggerKey(DIK_SPACE)) {
				//タイトルシーンへ
				uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("black.png");
				SceneManager::GetInstance()->SetNextScene("Title", SceneTransitionAnimation::Type::FADE, SceneTransitionAnimation::Type::FADE, SceneTransitionAnimation::Option::NONE, 1.0f, textureHandle);
			}
			//上
			if (input_->TriggerKey(DIK_W)) {
				//メニューを操作ガイドに切り替え
				selectMenu_ = Menu::kOperationGuide;
			}

			//色を変える
			stringMenuSprite_[1]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
			stringMenuSprite_[2]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
			stringMenuSprite_[3]->SetColor({ 0.929f,0.592f,0.255f,1.0f });


			break;
		}
		default:
			break;
		}
	}
	else {
		//全て非表示
		maskSprite_->SetIsDisplay(false);
		guideSprite_->SetIsDisplay(false);
		for (int i = 0; i < kMaxStringNum_; i++) {
			stringMenuSprite_[i]->SetIsDisplay(false);
		}
	}

	//操作ガイド表示中の処理
	if (isOperationGuideDisplay_) {
		//操作ガイドのみ表示
		maskSprite_->SetIsDisplay(false);
		guideSprite_->SetIsDisplay(true);
		for (int i = 0; i < kMaxStringNum_; i++) {
			stringMenuSprite_[i]->SetIsDisplay(false);
		}
		//スペースキーで終了
		if (input_->TriggerKey(DIK_TAB)) {
			//フラグを取り下げる
			isOperationGuideDisplay_ = false;
		}
	}
	else {
		//操作ガイドを非表示
		guideSprite_->SetIsDisplay(false);
	}
}

void PauseSystem::Debug() {

}
