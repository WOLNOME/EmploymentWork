#pragma once
#include "JsonUtil.h"
#include <vector>
#include <memory>

//アプリケーション
#include "application/object/character/weapon/enemy/EnemyCannon.h"
#include "application/object/character/weapon/enemy/EnemyBomb.h"

class EnemyManager;
class Player;
class PlayerUI;

/// <summary>
/// 敵の使う武器全般の管理を行うクラス
/// </summary>
class EnemyWeaponManager {
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
	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui();

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// 砲弾のコンテナを取得する
	/// </summary>
	/// <returns>砲弾のコンテナ</returns>
	const std::vector<std::unique_ptr<EnemyCannon>>& GetCannons() const { return cannons_; }
	/// <summary>
	/// 爆弾のコンテナを取得する
	/// </summary>
	/// <returns>爆弾のコンテナ</returns>
	const std::vector<std::unique_ptr<EnemyBomb>>& GetBombs() const { return bombs_; }

	////銃弾のコンテナ
	//const std::vector<std::unique_ptr<EnemyBullet>>& GetBullets() const { return bullets_; }


	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// エネミーマネージャーを設定する
	/// </summary>
	/// <param name="_enemyManager">設定するエネミーマネージャー</param>
	void SetEnemyManager(EnemyManager* _enemyManager) { enemyManager_ = _enemyManager; }
	/// <summary>
	/// プレイヤーを設定する
	/// </summary>
	/// <param name="_player">設定するプレイヤー</param>
	void SetPlayer(Player* _player) { player_ = _player; }
	/// <summary>
	/// プレイヤーUIを設定する
	/// </summary>
	/// <param name="_playerUI">設定するプレイヤーUI</param>
	void SetPlayerUI(PlayerUI* _playerUI);

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// 砲弾を生成する
	/// </summary>
	void CreateCannon();
	/// <summary>
	/// 爆弾を生成する
	/// </summary>
	void CreateBomb();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//エネミーマネージャー
	EnemyManager* enemyManager_ = nullptr;
	//プレイヤー
	Player* player_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json param_;

	//砲弾のコンテナ
	std::vector<std::unique_ptr<EnemyCannon>> cannons_;
	//爆弾のコンテナ
	std::vector<std::unique_ptr<EnemyBomb>> bombs_;

	////銃弾のコンテナ
	//std::vector<std::unique_ptr<EnemyBullet>> bullets_;

};

