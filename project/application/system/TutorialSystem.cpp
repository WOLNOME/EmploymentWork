#include "TutorialSystem.h"
#include <TextureManager.h>
#include <SpriteManager.h>
#include <SceneManager.h>
#include <Input.h>

using namespace Norm;

void TutorialSystem::Initialize() {
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
				sceneManager->SetNextScene("GamePlay", SceneTransitionAnimation::Type::SLIDEDOWN, SceneTransitionAnimation::Type::SLIDEUP, SceneTransitionAnimation::Option::SHAKE, 1.0f, TextureManager::GetInstance()->LoadTexture("shutter.png"));
			}

			return;
		}


		//スプライトの画像を変更
		sprite_->SetUVScrollSheetNum(i);


	}


}

void TutorialSystem::DebugWithImGui() {

}
