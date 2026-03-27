#pragma once
#include <Sprite.h>
#include <memory>

//前方宣言（エンジン）
namespace Norm {
	class Input;
	class SceneManager;
}
//前方宣言（アプリケーション）
class TimeScaleManager;
class Player;
class EnemyManager;

/// <summary>
/// エンド演出を管理するクラス
/// </summary>
class EndDirection {
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
	///		getter
	/// ============================== ///


	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// タイムスケールマネージャーを設定する
	/// </summary>
	/// <param name="_timeScaleManager">タイムスケールマネージャー</param>
	void SetTimeScaleManager(TimeScaleManager* _timeScaleManager) { timeScaleManager_ = _timeScaleManager; }
	/// <summary>
	/// プレイヤーを設定する
	/// </summary>
	/// <param name="_player">プレイヤー</param>
	void SetPlayer(Player* _player) { player_ = _player; }
	/// <summary>
	/// 敵マネージャーを設定する
	/// </summary>
	/// <param name="_enemyManager">敵マネージャー</param>
	void SetEnemyManager(EnemyManager* _enemyManager) { enemyManager_ = _enemyManager; }

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// シーン変更関連処理
	/// </summary>
	void SceneChange();

	//敵のボスを全滅させたときの処理
	void AllBossDefeated();


	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//インプット
	Norm::Input* input_ = nullptr;
	//シーンマネージャー
	Norm::SceneManager* sceneManager_ = nullptr;
	//タイムスケールマネージャー
	TimeScaleManager* timeScaleManager_ = nullptr;
	//プレイヤーのインスタンス
	Player* player_ = nullptr;
	//敵マネージャーのインスタンス
	EnemyManager* enemyManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//スローモーション中かどうかのフラグ
	bool isSlowMotion_ = false;
	//スローモーション開始後何秒間で遷移を開始するかの時間
	const float kSlowMotionToSceneChangeTime_ = 2.0f;
	//スローモーション中に測るタイマー
	float slowMotionTimer_ = 0.0f;
};

