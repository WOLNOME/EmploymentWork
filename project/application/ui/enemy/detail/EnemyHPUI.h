#pragma once
#include <JsonUtil.h>
#include <Sprite.h>
#include <array>
#include <memory>

class GameCamera;
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
	/// ゲームカメラのセット
	/// </summary>
	/// <param name="_camera">ゲームカメラ</param>
	void SetGameCamera(GameCamera* _camera) { camera_ = _camera; }
private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//エネミー
	EnemyManager* enemyManager_ = nullptr;
	//カメラ
	GameCamera* camera_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json param_;

	//HPバー
	static const int kNumHPBar = 10; //一度に表示可能なHPバーの数
	std::array<uint32_t, 2> textureHandles_;
	std::array<std::array<std::unique_ptr<Sprite>, 2>, kNumHPBar> sprites_;
	std::array<bool, kNumHPBar> isHPBarVisible_; //各HPバーの表示状態 
	float hpBarWidth_ = 0.0f;
	float hpBarHeight_ = 0.0f;

};

