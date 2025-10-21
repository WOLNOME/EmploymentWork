#pragma once
#include "BaseScene.h"
#include "GameCamera.h"

//アプリケーション
#include "application/object/environment/Skydome.h"
#include "application/object/environment/Ground.h"
#include <application/system/GameOverSystem.h>

/// <summary>
/// ゲームオーバーシーン全般を管理するクラス
/// </summary>
class GameOverScene : public BaseScene {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 終了時
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// デバッグ処理
	/// </summary>
	void DebugWithImGui() override;

private:
	//インプット
	Input* input_ = nullptr;

	//ゲーム用カメラ
	std::unique_ptr<GameCamera> camera_ = nullptr;
private://ライト

private://オブジェクト
	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//地面
	std::unique_ptr<Ground> ground_ = nullptr;
private://システム
	std::unique_ptr<GameOverSystem> gameOverSystem_ = nullptr;


};

