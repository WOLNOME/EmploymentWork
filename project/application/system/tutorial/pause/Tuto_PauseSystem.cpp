#include "Tuto_PauseSystem.h"
#include <Input.h>
#include <SceneManager.h>
#include <TextureManager.h>
#include <SpriteManager.h>
#include <TextTextureManager.h>
#include <WinApp.h>

using namespace Norm;

Tuto_PauseSystem::Tuto_PauseSystem() {
}

Tuto_PauseSystem::~Tuto_PauseSystem() {
}

void Tuto_PauseSystem::Initialize() {
	//SEの初期化
	decideSE_ = std::make_unique<Audio>();
	decideSE_->Initialize("se/decide.mp3");
	cursolMoveSE_ = std::make_unique<Audio>();
	cursolMoveSE_->Initialize("se/cursolMove.mp3");

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
				TextTextureManager::GetInstance()->EditTextSize(stringMenuTextureHandle_[i], 48.0f);
				stringMenuSprite_[i]->SetPosition({ WinApp::kClientWidth / 2.0f, 170.0f });
				break;
			}
			case 1: {
				//続けるテキスト
				TextTextureManager::GetInstance()->EditTextString(stringMenuTextureHandle_[i], L"続ける");
				stringMenuSprite_[i]->SetPosition({ WinApp::kClientWidth / 2.0f, 350.0f });
				break;
			}
			case 2: {
				//操作ガイドテキスト
				TextTextureManager::GetInstance()->EditTextString(stringMenuTextureHandle_[i], L"操作ガイド");
				stringMenuSprite_[i]->SetPosition({ WinApp::kClientWidth / 2.0f, 410.0f });
				break;
			}
			case 3: {
				// 移動チュートリアル
				TextTextureManager::GetInstance()->EditTextString(
					stringMenuTextureHandle_[i], L"移動チュートリアル");
				stringMenuSprite_[i]->SetPosition({ WinApp::kClientWidth / 2.0f, 470.0f });
				break;
			}
			case 4: {
				// 攻撃チュートリアル
				TextTextureManager::GetInstance()->EditTextString(
					stringMenuTextureHandle_[i], L"攻撃チュートリアル");
				stringMenuSprite_[i]->SetPosition({ WinApp::kClientWidth / 2.0f, 530.0f });
				break;
			}
			case 5: {
				// チュートリアルをスキップ
				TextTextureManager::GetInstance()->EditTextString(
					stringMenuTextureHandle_[i], L"チュートリアルをスキップ");
				stringMenuSprite_[i]->SetPosition({ WinApp::kClientWidth / 2.0f, 590.0f });
				break;
			}
			case 6: {
				// タイトルへ
				TextTextureManager::GetInstance()->EditTextString(
					stringMenuTextureHandle_[i], L"タイトルに戻る");
				stringMenuSprite_[i]->SetPosition({ WinApp::kClientWidth / 2.0f, 650.0f });
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

void Tuto_PauseSystem::Update() {
    Input* input = Input::GetInstance();

    // ポーズ切り替え
    if (input->TriggerKey(DIK_ESCAPE) || input->TriggerPadButton(GamePadButton::START)) {
        isPause_ = !isPause_;
        isOperationGuideDisplay_ = false;
        decideSE_->Play(false, 1.0f);
    }

    if (isPause_) {

        // =============================
        // 操作ガイド表示中
        // =============================
        if (isOperationGuideDisplay_) {

            maskSprite_->SetIsDisplay(false);
            guideSprite_->SetIsDisplay(true);

            for (int i = 0; i < kMaxStringNum_; i++) {
                stringMenuSprite_[i]->SetIsDisplay(false);
            }

            if (input->TriggerKey(DIK_SPACE) ||
                input->TriggerPadButton(GamePadButton::A)) {

                isOperationGuideDisplay_ = false;
                decideSE_->Play(false, 1.0f);
            }
        }
        else {

            // =============================
            // ポーズメニュー表示
            // =============================
            maskSprite_->SetIsDisplay(true);
            guideSprite_->SetIsDisplay(false);

            for (int i = 0; i < kMaxStringNum_; i++) {
                stringMenuSprite_[i]->SetIsDisplay(true);
            }

            switch (selectMenu_) {

                //================================================
            case Menu::kContinue:
                //================================================
            {
                if (input->TriggerKey(DIK_SPACE) || input->TriggerPadButton(GamePadButton::A)) {
                    isPause_ = false;
                    decideSE_->Play(false, 1.0f);
                }

                if (input->TriggerKey(DIK_S) ||
                    input->TriggerPadButton(GamePadButton::DPAD_DOWN) ||
                    input->GetLStickDir().y < 0.0f) {

                    selectMenu_ = Menu::kOperationGuide;
                    cursolMoveSE_->Play(false, 1.0f);
                }

                stringMenuSprite_[1]->SetColor({ 0.929f,0.592f,0.255f,1 });
                stringMenuSprite_[2]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[3]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[4]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[5]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[6]->SetColor({ 1,1,1,1 });
                break;
            }

            //================================================
            case Menu::kOperationGuide:
                //================================================
            {
                if (input->TriggerKey(DIK_SPACE) || input->TriggerPadButton(GamePadButton::A)) {
                    isOperationGuideDisplay_ = true;
                    decideSE_->Play(false, 1.0f);
                }

                if (input->TriggerKey(DIK_W) ||
                    input->TriggerPadButton(GamePadButton::DPAD_UP) ||
                    input->GetLStickDir().y > 0.0f) {

                    selectMenu_ = Menu::kContinue;
                    cursolMoveSE_->Play(false, 1.0f);
                }

                if (input->TriggerKey(DIK_S) ||
                    input->TriggerPadButton(GamePadButton::DPAD_DOWN) ||
                    input->GetLStickDir().y < 0.0f) {

                    selectMenu_ = Menu::kMoveTuto;
                    cursolMoveSE_->Play(false, 1.0f);
                }

                stringMenuSprite_[1]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[2]->SetColor({ 0.929f,0.592f,0.255f,1 });
                stringMenuSprite_[3]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[4]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[5]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[6]->SetColor({ 1,1,1,1 });
                break;
            }

            //================================================
            case Menu::kMoveTuto:
                //================================================
            {
                if (input->TriggerKey(DIK_SPACE) || input->TriggerPadButton(GamePadButton::A)) {
                    decideSE_->Play(false, 1.0f);
                }

                if (input->TriggerKey(DIK_W) ||
                    input->TriggerPadButton(GamePadButton::DPAD_UP) ||
                    input->GetLStickDir().y > 0.0f) {

                    selectMenu_ = Menu::kOperationGuide;
                    cursolMoveSE_->Play(false, 1.0f);
                }

                if (input->TriggerKey(DIK_S) ||
                    input->TriggerPadButton(GamePadButton::DPAD_DOWN) ||
                    input->GetLStickDir().y < 0.0f) {

                    selectMenu_ = Menu::kAttackTuto;
                    cursolMoveSE_->Play(false, 1.0f);
                }

                stringMenuSprite_[1]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[2]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[3]->SetColor({ 0.929f,0.592f,0.255f,1 });
                stringMenuSprite_[4]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[5]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[6]->SetColor({ 1,1,1,1 });
                break;
            }

            //================================================
            case Menu::kAttackTuto:
                //================================================
            {
                if (input->TriggerKey(DIK_SPACE) || input->TriggerPadButton(GamePadButton::A)) {
                    decideSE_->Play(false, 1.0f);
                }

                if (input->TriggerKey(DIK_W) ||
                    input->TriggerPadButton(GamePadButton::DPAD_UP) ||
                    input->GetLStickDir().y > 0.0f) {

                    selectMenu_ = Menu::kMoveTuto;
                    cursolMoveSE_->Play(false, 1.0f);
                }

                if (input->TriggerKey(DIK_S) ||
                    input->TriggerPadButton(GamePadButton::DPAD_DOWN) ||
                    input->GetLStickDir().y < 0.0f) {

                    selectMenu_ = Menu::kSkip;
                    cursolMoveSE_->Play(false, 1.0f);
                }

                stringMenuSprite_[1]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[2]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[3]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[4]->SetColor({ 0.929f,0.592f,0.255f,1 });
                stringMenuSprite_[5]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[6]->SetColor({ 1,1,1,1 });
                break;
            }

            //================================================
            case Menu::kSkip:
                //================================================
            {
                if (input->TriggerKey(DIK_SPACE) || input->TriggerPadButton(GamePadButton::A)) {

                    uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("shutter.png");

                    if (SceneManager::GetInstance()->SetNextScene("GamePlay", SceneTransitionAnimation::Type::SLIDEDOWN, SceneTransitionAnimation::Type::SLIDEUP, SceneTransitionAnimation::Option::SHAKE, 1.0f,
                        textureHandle)) {

                        decideSE_->Play(false, 1.0f);
                    }
                }

                if (input->TriggerKey(DIK_W) ||
                    input->TriggerPadButton(GamePadButton::DPAD_UP) ||
                    input->GetLStickDir().y > 0.0f) {

                    selectMenu_ = Menu::kAttackTuto;
                    cursolMoveSE_->Play(false, 1.0f);
                }

                if (input->TriggerKey(DIK_S) ||
                    input->TriggerPadButton(GamePadButton::DPAD_DOWN) ||
                    input->GetLStickDir().y < 0.0f) {

                    selectMenu_ = Menu::kTItle;
                    cursolMoveSE_->Play(false, 1.0f);
                }

                stringMenuSprite_[1]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[2]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[3]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[4]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[5]->SetColor({ 0.929f,0.592f,0.255f,1 });
                stringMenuSprite_[6]->SetColor({ 1,1,1,1 });
                break;
            }

            //================================================
            case Menu::kTItle:
                //================================================
            {
                if (input->TriggerKey(DIK_SPACE) || input->TriggerPadButton(GamePadButton::A)) {

                    uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("shutter.png");

                    if (SceneManager::GetInstance()->SetNextScene("Title", SceneTransitionAnimation::Type::SLIDEDOWN, SceneTransitionAnimation::Type::SLIDEUP, SceneTransitionAnimation::Option::SHAKE, 1.0f,
                        textureHandle)) {

                        decideSE_->Play(false, 1.0f);
                    }
                }

                if (input->TriggerKey(DIK_W) ||
                    input->TriggerPadButton(GamePadButton::DPAD_UP) ||
                    input->GetLStickDir().y > 0.0f) {

                    selectMenu_ = Menu::kSkip;
                    cursolMoveSE_->Play(false, 1.0f);
                }

                stringMenuSprite_[1]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[2]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[3]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[4]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[5]->SetColor({ 1,1,1,1 });
                stringMenuSprite_[6]->SetColor({ 0.929f,0.592f,0.255f,1 });
                break;
            }
            }
        }
    }
    else {
        maskSprite_->SetIsDisplay(false);
        guideSprite_->SetIsDisplay(false);

        for (int i = 0; i < kMaxStringNum_; i++) {
            stringMenuSprite_[i]->SetIsDisplay(false);
        }
    }
}

void Tuto_PauseSystem::Debug() {

}
