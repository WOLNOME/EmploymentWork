#pragma once
#include <JsonUtil.h>
#include <Sprite.h>
#include <Vector2.h>
#include <memory>
#include <array>

//前方宣言（アプリケーション）
class CameraManager;
class Player;

/// <summary>
/// 移動インジケーターを管理するクラス
/// </summary>
class MoveIndicator {
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

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	/// <param name="_player">プレイヤー</param>
	void SetPlayer(Player* _player) { player_ = _player; }
	/// <summary>
	/// カメラマネージャーの設定
	/// </summary>
	/// <param name="_cameraManager">カメラマネージャー</param>
	void SetCameraManager(CameraManager* _cameraManager) { cameraManager_ = _cameraManager; }

private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	Player* player_ = nullptr;
	CameraManager* cameraManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json playerParam_;
	json playerUIParam_;

	//テクスチャハンドル
	uint32_t textureHadle_ = 0u;
	//スプライト
	std::unique_ptr<Norm::Sprite> sprite_ = nullptr;

};

