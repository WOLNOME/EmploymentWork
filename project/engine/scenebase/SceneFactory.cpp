#include "SceneFactory.h"
#include "DevelopScene.h"
#include "ParticleEditorScene.h"

//アプリケーション
#include <application/scene/GamePlayScene.h>
#include <application/scene/GameOverScene.h>

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
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
