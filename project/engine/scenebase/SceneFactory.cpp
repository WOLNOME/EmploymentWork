#include "SceneFactory.h"
#include "ParticleEditorScene.h"

//アプリケーション
#include <application/scene/TitleScene.h>
#include <application/scene/GamePlayScene.h>
#include <application/scene/GameClearScene.h>
#include <application/scene/GameOverScene.h>
#include <application/scene/DIR_BossAppearScene.h>
#include <application/scene/DIR_BossDestroyScene.h>

namespace Norm {

	std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName) {
		//次のシーンを生成
		std::unique_ptr<BaseScene> newScene = nullptr;

		if (sceneName == "Title") {
			newScene = std::make_unique<TitleScene>();
		}
		else if (sceneName == "GamePlay") {
			newScene = std::make_unique<GamePlayScene>();
		}
		else if (sceneName == "GameClear") {
			newScene = std::make_unique<GameClearScene>();
		}
		else if (sceneName == "GameOver") {
			newScene = std::make_unique<GameOverScene>();
		}
		else if(sceneName == "DIR_BossAppear") {
			newScene = std::make_unique<DIR_BossAppearScene>();
		}
		else if (sceneName == "DIR_BossDestroy") {
			newScene = std::make_unique<DIR_BossDestroyScene>();
		}

		// パーティクルエディター
		else if (sceneName == "PARTICLEEDITOR") {
			newScene = std::make_unique<ParticleEditorScene>();
		}

		//シーン名をセット
		newScene->SetSceneName(sceneName);

		return newScene;
	}

}