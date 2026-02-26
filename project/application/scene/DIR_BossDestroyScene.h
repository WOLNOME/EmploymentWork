#pragma once
#include "BaseScene.h"

//アプリケーション
#include "application/system/CameraManager.h"
#include "application/object/environment/Skydome.h"
#include "application/object/environment/Ground.h"
#include "application/system/BossDestroySystem.h"

/// <summary>
/// ボス撃破演出シーン全般を管理するクラス
/// </summary>
class DIR_BossDestroyScene : public Norm::BaseScene {
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

	//平行光源
	std::unique_ptr<Norm::DirectionalLight> dirLight_;

	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//地面
	std::unique_ptr<Ground> ground_ = nullptr;

	//ボス撃破演出システム
	std::unique_ptr<BossDestroySystem> bossDestroySystem_ = nullptr;


};

