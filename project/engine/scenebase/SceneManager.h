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
	/// ============================== ///
	///		メンバ関数
	///	============================== ///

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static SceneManager* GetInstance();

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

	/// <summary>
	/// 次のシーンのセット
	/// </summary>
	/// <param name="nextSceneName"></param>
	/// <param name="inType"></param>
	/// <param name="outType"></param>
	/// <param name="option"></param>
	/// <param name="time"></param>
	/// <param name="_textureHandle"></param>
	void SetNextScene(const std::string& nextSceneName, SceneTransitionAnimation::Type inType = SceneTransitionAnimation::Type::FADE, SceneTransitionAnimation::Type outType = SceneTransitionAnimation::Type::FADE, SceneTransitionAnimation::Option option = SceneTransitionAnimation::Option::NONE, float time = 1.0f, uint32_t _textureHandle = 0u);

private:
	/// ============================== ///
	///		非公開メンバ関数
	///	============================== ///

	/// <summary>
	/// シーン切り替え
	/// </summary>
	void ChangeScene();

	/// ============================== ///
	///		メンバ変数
	///	============================== ///

	//今のシーン
	BaseScene* scene_ = nullptr;
	//次のシーン
	BaseScene* nextScene_ = nullptr;
	//シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;
	//シーン遷移アニメーション
	std::unique_ptr<SceneTransitionAnimation> sceneTransitionAnimation_ = nullptr;

};

