#pragma once
#include "BaseScene.h"

//アプリケーション
#include "application/system/CameraManager.h"
#include "application/object/environment/Skydome.h"
#include "application/object/environment/Ground.h"
#include <application/system/GameClearSystem.h>

/// <summary>
/// ゲームクリアシーン全般を管理するクラス
/// </summary>
class GameClearScene : public Norm::BaseScene {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

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
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//カメラマネージャー
	std::unique_ptr<CameraManager> cameraManager_ = nullptr;

	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//地面
	std::unique_ptr<Ground> ground_ = nullptr;

	//ゲームクリアシステム
	std::unique_ptr<GameClearSystem> gameClearSystem_ = nullptr;


};

