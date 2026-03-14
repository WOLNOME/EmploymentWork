#pragma once
#include <Input.h>
#include <Audio.h>
#include <SceneManager.h>
#include <Object3d.h>
#include <Sprite.h>
#include <memory>
#include <Handle.h>

/// <summary>
/// ゲームオーバーシーンのUIや演出、操作等を管理するクラス
/// </summary>
class GameOverSystem {
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
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// 操作処理
	/// </summary>
	void Operate();
	/// <summary>
	/// 演出処理
	/// </summary>
	void DirectionUI();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//インプット
	Norm::Input* input_ = nullptr;
	//シーンマネージャー
	Norm::SceneManager* sceneManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//BGM
	std::unique_ptr<Norm::Audio> bgm_ = nullptr;
	//SE
	std::unique_ptr<Norm::Audio> decideSE_ = nullptr;		//決定

	//テキスト操作変数
	bool isHalfPeriod_ = false;
	float timer_ = 0.0f;
	const float kTime_ = 2.0f;

	//ゲームオーバーテキスト
	Norm::Handle gameOverTextHandle_;
	std::unique_ptr<Norm::Sprite> gameOverTextSprite_ = nullptr;

	//タイトルに戻るテキスト
	uint32_t titleTextHandle_;
	std::unique_ptr<Norm::Sprite> titleTextSprite_ = nullptr;

};

