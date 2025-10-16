#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"
#include "SceneTransitionAnimation.h"
#include "Audio.h"
#include <string>
#include <memory>

/// <summary>
/// 全てのシーン処理を行うクラス
/// シングルトンパターンで実装
/// </summary>
class SceneManager {
private://コンストラクタ等の隠蔽
	static SceneManager* instance;

	SceneManager() = default;//コンストラクタ隠蔽
	~SceneManager() = default;//デストラクタ隠蔽
	SceneManager(SceneManager&) = delete;//コピーコンストラクタ封印
	SceneManager& operator=(SceneManager&) = delete;//コピー代入演算子封印
public:
	//シングルトンインスタンスの取得
	static SceneManager* GetInstance();
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// デバッグ処理
	/// </summary>
	void DebugWithImGui();
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

private:
	//シーン切り替え
	void ChangeScene();
public:
	//次シーンのセット
	void SetNextScene(const std::string& nextSceneName, SceneTransitionAnimation::Type inType = SceneTransitionAnimation::Type::FADE, SceneTransitionAnimation::Type outType = SceneTransitionAnimation::Type::FADE, SceneTransitionAnimation::Option option = SceneTransitionAnimation::Option::NONE, float time = 1.0f, uint32_t _textureHandle = 0u);

private:
	//今のシーン
	BaseScene* scene_ = nullptr;
	//次のシーン
	BaseScene* nextScene_ = nullptr;
	//シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;
	//シーン遷移アニメーション
	std::unique_ptr<SceneTransitionAnimation> sceneTransitionAnimation_ = nullptr;

};

