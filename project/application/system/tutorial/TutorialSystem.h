#pragma once
#include <Audio.h>
#include <Sprite.h>
#include <memory>

//アプリケーション
#include <application/system/tutorial/manager/TutorialManager.h>

/// <summary>
/// チュートリアルシーンのUIや演出、操作等を管理するクラス
/// </summary>
class TutorialSystem {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui();

private:

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//BGM
	std::unique_ptr<Norm::Audio> bgm_ = nullptr;

	//チュートリアルマネージャー
	std::unique_ptr<TutorialManager> tutorialManager_ = nullptr;

};

