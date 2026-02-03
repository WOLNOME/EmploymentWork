#include "SceneFactory.h"
#include "ParticleEditorScene.h"

//アプリケーション
#include <application/scene/TitleScene.h>
#include <application/scene/GamePlayScene.h>
#include <application/scene/GameClearScene.h>
#include <application/scene/GameOverScene.h>

namespace Norm {

	BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
		//次のシーンを生成
		BaseScene* newScene = nullptr;

		if (sceneName == "Title") {
			newScene = new TitleScene();
		}
		else if (sceneName == "GamePlay") {
			newScene = new GamePlayScene();
		}
		else if (sceneName == "GameClear") {
			newScene = new GameClearScene();
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

}