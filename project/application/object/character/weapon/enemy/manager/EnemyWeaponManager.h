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
	//初期化
	void Initialize();
	//更新
	void Update();

	//デバッグ用パラメーター調整
	void DebugWithImGui();

public://getter
	//砲弾のコンテナ
	const std::vector<std::unique_ptr<EnemyCannon>>& GetCannons() const { return cannons_; }
	//爆弾のコンテナ
	const std::vector<std::unique_ptr<EnemyBomb>>& GetBombs() const { return bombs_; }

	////銃弾のコンテナ
	//const std::vector<std::unique_ptr<EnemyBullet>>& GetBullets() const { return bullets_; }

public://setter
	//エネミーマネージャーセット
	void SetEnemyManager(EnemyManager* _enemyManager) { enemyManager_ = _enemyManager; }
	//プレイヤーセット
	void SetPlayer(Player* _player) { player_ = _player; }
	//プレイヤーUIセット
	void SetPlayerUI(PlayerUI* _playerUI);

private:
	//砲弾の生成
	void CreateCannon();
	//爆弾の生成
	void CreateBomb();

private:
	//エネミーマネージャー
	EnemyManager* enemyManager_ = nullptr;
	//プレイヤー
	Player* player_ = nullptr;

private:
	//パラメーター
	json param_;

	//砲弾のコンテナ
	std::vector<std::unique_ptr<EnemyCannon>> cannons_;
	//爆弾のコンテナ
	std::vector<std::unique_ptr<EnemyBomb>> bombs_;

	////銃弾のコンテナ
	//std::vector<std::unique_ptr<EnemyBullet>> bullets_;

};

