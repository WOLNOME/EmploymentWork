#include "SceneFactory.h"
#include "DevelopScene.h"
#include "GamePlayScene.h"
#include "ParticleCreatorScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//次のシーンを生成
	BaseScene* newScene = nullptr;

	if (sceneName == "DEVELOP") {
		newScene = new DevelopScene();
	}
	else if (sceneName == "GamePlay") {
		newScene = new GamePlayScene();
	}
	//パーティクルクリエイター
	else if (sceneName == "PARTICLECREATOR") {
		newScene = new ParticleCreatorScene();
	}

	return newScene;
}
