#pragma once
#include <Input.h>
#include <SceneManager.h>
#include <Object3d.h>
#include <Sprite.h>
#include <memory>
#include <Handle.h>

/// <summary>
/// ゲームクリアシーンのUIや演出、操作等を管理するクラス
/// </summary>
class GameClearSystem {
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
	Input* input_ = nullptr;
	//シーンマネージャー
	SceneManager* sceneManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//テキスト操作変数
	bool isHalfPeriod_ = false;
	float timer_ = 0.0f;
	const float time_ = 2.0f;

	//クリアテキスト
	Handle clearTextHandle_;
	std::unique_ptr<Sprite> clearTextSprite_ = nullptr;

	//タイトルに戻るテキスト
	Handle titleTextHandle_;
	std::unique_ptr<Sprite> titleTextSprite_ = nullptr;

};

