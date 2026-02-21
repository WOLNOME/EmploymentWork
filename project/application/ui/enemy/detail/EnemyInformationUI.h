#pragma once
#include <JsonUtil.h>
#include <Sprite.h>
#include <array>
#include <memory>
#include <Handle.h>

//前方宣言（アプリケーション）
class CameraManager;
class EnemyManager;

/// <summary>
/// 敵の情報UIを管理するクラス
/// </summary>
class EnemyInformationUI {
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
	///		メンバ関数（private:）
	/// ============================== ///

	//情報処理
	void InformationProcess();

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

	//目標フレーム
	static const int kNumEnemyFrame = 10;
	uint32_t thEnemyFrame_;
	std::array<std::unique_ptr<Norm::Sprite>, kNumEnemyFrame> spriteEnemyFrame_;

	//詳細情報基盤
	uint32_t thInfoBase_;
	std::unique_ptr<Norm::Sprite> spriteInfoBase_;
	//レベル
	uint32_t thLevel_;
	std::unique_ptr<Norm::Sprite> spriteLevel_;
	//距離
	Norm::Handle thRange_;
	std::unique_ptr<Norm::Sprite> spriteRange_;
	//アイテム
	static const int kNumItem = 3; //一度に表示可能なアイテムの数
	uint32_t thItem_;
	std::array<std::unique_ptr<Norm::Sprite>, kNumItem> spriteItem_;

};

