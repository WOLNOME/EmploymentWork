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

	//チュートリアルマネージャーの初期化
	tutorialManager_ = std::make_unique<TutorialManager>();
	tutorialManager_->Initialize();


}

void TutorialSystem::Update() {

}

void TutorialSystem::DebugWithImGui() {

}
