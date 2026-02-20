#pragma once
#include <JsonUtil.h>
#include <Sprite.h>
#include <array>
#include <memory>
#include <Handle.h>

//前方宣言（エンジン）
namespace Norm {
	class GameCamera;
}
//前方宣言（アプリケーション）
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
	/// ゲームカメラのセット
	/// </summary>
	/// <param name="_camera">ゲームカメラ</param>
	void SetGameCamera(Norm::GameCamera* _camera) { camera_ = _camera; }
private:
	/// ============================== ///
	///		メンバ関数（private:）
	/// ============================== ///

	//情報処理
	void InformationProcess();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//エネミー
	EnemyManager* enemyManager_ = nullptr;
	//カメラ
	Norm::GameCamera* camera_ = nullptr;

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

