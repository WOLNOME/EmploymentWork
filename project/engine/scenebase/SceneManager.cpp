#include "SceneManager.h"
#include "SceneFactory.h"
#include "ImGuiManager.h"
#include <cassert>

namespace Norm {

	std::unique_ptr<SceneManager> SceneManager::instance_ = nullptr;

	SceneManager* SceneManager::GetInstance() {
		if (!instance_) {
			instance_ = std::unique_ptr<SceneManager>(new SceneManager());
		}
		return instance_.get();
	}

	void SceneManager::Initialize() {
		//シーンファクトリーの生成
		sceneFactory_ = std::make_unique<SceneFactory>();
		//シーン遷移アニメーションの生成
		sceneTransitionAnimation_ = std::make_unique<SceneTransitionAnimation>();
		sceneTransitionAnimation_->Initialize();

	}

	void SceneManager::Update() {
		//シーン遷移アニメーションの更新
		sceneTransitionAnimation_->Update();
		//シーン切り替え処理
		ChangeScene();
		//実行中シーンを更新する
		scene_->Update();

	}

	void SceneManager::DebugWithImGui() {
#ifdef _DEBUG
		//FPS表示
		scene_->ShowFPS();
		//デバッグ処理
		scene_->DebugWithImGui();
		//ブレークポイント
		if (Input::GetInstance()->TriggerKey(DIK_F5)) {
			int a = 0;
			a++;
		}

#endif // _DEBUG

	}

	void SceneManager::Finalize() {
		//最後のシーンの終了と解放
		scene_->Finalize();
		scene_.reset();
		//シーンファクトリー解放
		sceneFactory_.reset();
		//インスタンスを削除
		instance_.reset();
	}

	void SceneManager::ChangeScene() {
		//次のシーン予約があるなら
		if (nextScene_ && !sceneTransitionAnimation_->GetIsTransitioning()) {
			//遷移アニメーション開始
			sceneTransitionAnimation_->StartTransition();
		}
		//遷移アニメーション中なら
		if (sceneTransitionAnimation_->GetState() == SceneTransitionAnimation::State::UPDATE_IN) {
			//フェードイン処理
			sceneTransitionAnimation_->UpdateIn();
		}
		else if (sceneTransitionAnimation_->GetState() == SceneTransitionAnimation::State::END_IN) {
			//フェードイン終了
			sceneTransitionAnimation_->EndIn();
			//旧シーンの終了
			if (scene_) {
				scene_->Finalize();
				scene_.reset();
			}
			//シーンの切り替え
			scene_ = std::move(nextScene_);
			nextScene_.reset();
			//次のシーンを初期化する
			scene_->Initialize();
		}
		else if (sceneTransitionAnimation_->GetState() == SceneTransitionAnimation::State::UPDATE_OUT) {
			//フェードアウト処理
			sceneTransitionAnimation_->UpdateOut();
		}
		else if (sceneTransitionAnimation_->GetState() == SceneTransitionAnimation::State::END_OUT) {
			//フェードアウト終了
			sceneTransitionAnimation_->EndOut();
		}
		else if (sceneTransitionAnimation_->GetState() == SceneTransitionAnimation::State::END_ALL) {
			//フェードアウト終了
			sceneTransitionAnimation_->EndAll();
		}
	}

	void SceneManager::SetNextScene(const std::string& nextSceneName, SceneTransitionAnimation::Type inType, SceneTransitionAnimation::Type outType, SceneTransitionAnimation::Option option, float time, uint32_t _textureHandle) {
		//遷移中なら何もしない
		if (sceneTransitionAnimation_->GetIsTransitioning()) return;

		//警告
		assert(sceneFactory_);
		assert(!nextScene_);

		//もし最初のシーンだったらここで生成＆初期化
		if (!scene_) {
			scene_ = sceneFactory_->CreateScene(nextSceneName);
			nextScene_.reset();
			scene_->Initialize();
			return;
		}

		//次シーンを生成
		nextScene_ = sceneFactory_->CreateScene(nextSceneName);
		//遷移アニメーションタイプを設定
		sceneTransitionAnimation_->SetType(inType, outType);
		//遷移アニメーションオプションを設定
		sceneTransitionAnimation_->SetOption(option);
		//遷移アニメーションも時間を設定
		sceneTransitionAnimation_->SetTime(time);
		//テクスチャを設定
		sceneTransitionAnimation_->SetTexture(_textureHandle);
	}

}