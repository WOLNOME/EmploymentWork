#pragma once
#include <memory>
#include <JsonUtil.h>
#include <Object3d.h>

//アプリケーション
#include "application/object/level/loader/LevelLoader.h"
#include "application/system/direction/cinematic/Cinematic.h"

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
	void SetCameraManager(CameraManager* _cameraManager);

private:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// 初期化時点でブラックボードに必要な情報を入れる
	/// </summary>
	void InitBlackBoard();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//カメラマネージャー
	CameraManager* cameraManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json param_;

	//レベルローダー
	std::unique_ptr<LevelLoader> levelLoader_ = nullptr;

	//鍵1
	std::unique_ptr<Norm::Object3d> key1_ = nullptr;
	Norm::WorldTransform wtKey1_;

	//鍵2
	std::unique_ptr<Norm::Object3d> key2_ = nullptr;
	Norm::WorldTransform wtKey2_;

	//ブラックボード
	std::unique_ptr<Norm::BlackBoard> blackBoard_ = nullptr;
	//映像演出
	std::unique_ptr<Cinematic> cinematic_ = nullptr;

};

