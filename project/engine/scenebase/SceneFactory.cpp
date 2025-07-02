#include "SceneFactory.h"
#include "DevelopScene.h"
#include "GamePlayScene.h"
#include "GameOverScene.h"
#include "ParticleEditorScene.h"

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
	else if (sceneName == "GameOver") {
		newScene = new GameOverScene();
	}
	//パーティクルエディター
	else if (sceneName == "PARTICLEEDITOR") {
		newScene = new ParticleEditorScene();
	}

	return newScene;
}
