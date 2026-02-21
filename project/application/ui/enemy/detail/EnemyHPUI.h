#pragma once
#include <JsonUtil.h>
#include <Sprite.h>
#include <array>
#include <memory>

//前方宣言（アプリケーション）
class CameraManager;
class EnemyManager;

/// <summary>
/// 敵のHPUIを管理するクラス
/// </summary>
class EnemyHPUI {
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
	/// エネミーマネージャーのセット
	/// </summary>
	/// <param name="_enemyManager">エネミーマネージャー</param>
	void SetEnemyManager(EnemyManager* _enemyManager) { enemyManager_ = _enemyManager; }
	/// <summary>
	/// カメラマネージャーのセット
	/// </summary>
	/// <param name="_cameraManager">カメラマネージャー</param>
	void SetCameraManager(CameraManager* _cameraManager) { cameraManager_ = _cameraManager; }
private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//カメラマネージャー
	CameraManager* cameraManager_ = nullptr;
	//エネミー
	EnemyManager* enemyManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json param_;

	//HPバー
	static const int kNumHPBar = 10; //一度に表示可能なHPバーの数
	std::array<uint32_t, 2> textureHandles_;
	std::array<std::array<std::unique_ptr<Norm::Sprite>, 2>, kNumHPBar> sprites_;
	std::array<bool, kNumHPBar> isHPBarVisible_; //各HPバーの表示状態 
	float hpBarWidth_ = 0.0f;
	float hpBarHeight_ = 0.0f;

};

