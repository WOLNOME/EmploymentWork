#include "TutorialSystem.h"
#include <TextureManager.h>
#include <SpriteManager.h>
#include <SceneManager.h>
#include <Input.h>

using namespace Norm;

void TutorialSystem::Initialize() {

	//BGMの初期化
	bgm_ = std::make_unique<Audio>();
	bgm_->Initialize("bgm/tutorial.mp3");
	bgm_->Play(true, 1.0f);
	//SEの初期化
	decideSE_ = std::make_unique<Audio>();
	decideSE_->Initialize("se/decide.mp3");

	//タイトルテキスト
	{
		//テクスチャハンドルに登録
		textureHandle_ = TextureManager::GetInstance()->LoadTexture("tutorialUI.png");
		//スプライト
		sprite_ = std::make_unique<Sprite>();
		sprite_->Initialize(UVScrollTag{}, SpriteManager::GetInstance()->GenerateName("tutorialUI"), Order::Front1,3,0.01f,false,textureHandle_);
		sprite_->SetPosition({ 640,360 });
		sprite_->SetAnchorPoint({ 0.5f,0.5f });
	}

}

void TutorialSystem::Update() {
	Input* input = Input::GetInstance();
	SceneManager* sceneManager = SceneManager::GetInstance();

	if (input->TriggerKey(DIK_SPACE) || input->TriggerPadButton(GamePadButton::A)) {
		//iをインクリメント
		i++;
		//もしiが3以上になったら
		if (i >= 3) {
			//ゲームプレイシーンに移行
			if (input->TriggerKey(DIK_SPACE) || input->TriggerPadButton(GamePadButton::A)) {
				if (sceneManager->SetNextScene("GamePlay", SceneTransitionAnimation::Type::SLIDEDOWN, SceneTransitionAnimation::Type::SLIDEUP, SceneTransitionAnimation::Option::SHAKE, 1.0f, TextureManager::GetInstance()->LoadTexture("shutter.png"))) {
					//決定音を出す
					decideSE_->Play(false, 1.0f);
				}
			}

			return;
		}

		//決定音を出す
		decideSE_->Play(false, 1.0f);

		//スプライトの画像を変更
		sprite_->SetUVScrollSheetNum(i);


	}


}

void TutorialSystem::DebugWithImGui() {

}
