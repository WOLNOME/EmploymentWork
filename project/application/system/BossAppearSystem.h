#pragma once
#include <memory>

//アプリケーション
#include "application/object/level/loader/LevelLoader.h"

//前方宣言
class CameraManager;

/// <summary>
/// ボス出現演出の処理を行うクラス
/// </summary>
class BossAppearSystem {
public:
	/// ============================== ///
	///		メンバ関数（public）
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

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// カメラマネージャーのセット
	/// </summary>
	/// <param name="_cameraManager">カメラマネージャー</param>
	void SetCameraManager(CameraManager* _cameraManager) { cameraManager_ = _cameraManager; }

private:
	/// ============================== ///
	///		メンバ変数（private）
	/// ============================== ///

	/// <summary>
	/// 演出
	/// </summary>
	void Direction();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//カメラマネージャー
	CameraManager* cameraManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//レベルローダー
	std::unique_ptr<LevelLoader> levelLoader_ = nullptr;



};

