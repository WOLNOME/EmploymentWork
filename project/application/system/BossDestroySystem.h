#pragma once
#include <Audio.h>
#include <memory>
#include <vector>
#include <JsonUtil.h>
#include <Sprite.h>
#include <Object3d.h>
#include <CombinedParticle.h>

//アプリケーション
#include "application/object/level/loader/LevelLoader.h"
#include "application/system/direction/cinematic/Cinematic.h"

//前方宣言
class CameraManager;

/// <summary>
/// ボス撃破演出の処理を行うクラス
/// </summary>
class BossDestroySystem {
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



	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//カメラマネージャー
	CameraManager* cameraManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//SE
	std::unique_ptr<Norm::Audio> decideSE_ = nullptr;		//決定

	//パラメーター
	json param_;

	//レベルローダー
	std::unique_ptr<LevelLoader> levelLoader_ = nullptr;

	//レターボックス
	uint32_t thLetterBox_ = 0u;
	std::unique_ptr<Norm::Sprite> letterBox_ = nullptr;

	//スキップUI
	uint32_t thSkipUI_ = 0u;
	std::unique_ptr<Norm::Sprite> skipUI_ = nullptr;

	//ボス
	std::unique_ptr<Norm::Object3d> boss_ = nullptr;
	std::unique_ptr<Norm::WorldTransform> wtBoss_;

	//黒煙パーティクル
	static const int kBSNum = 3;
	std::vector<std::unique_ptr<Norm::CombinedParticle>> blackSmokes_{ kBSNum };

	//中規模爆発パーティクル
	static const int kMENum = 15;
	std::vector<std::unique_ptr<Norm::CombinedParticle>> mediumExplosions_{ kMENum };

	//巨大爆発パーティクル
	static const int kHENum = 3;
	std::vector<std::unique_ptr<Norm::CombinedParticle>> hugeExplosions_{ kHENum };



	//ブラックボード
	std::unique_ptr<Norm::BlackBoard> blackBoard_ = nullptr;
	//映像演出
	std::unique_ptr<Cinematic> cinematic_ = nullptr;

};

