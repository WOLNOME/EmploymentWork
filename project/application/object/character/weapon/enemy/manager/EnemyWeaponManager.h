#pragma once
#include "JsonUtil.h"
#include <vector>
#include <memory>

//アプリケーション
#include "application/object/character/weapon/enemy/EnemyCannon.h"

class EnemyManager;
class Player;
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
	////銃弾のコンテナ
	//const std::vector<std::unique_ptr<EnemyBullet>>& GetBullets() const { return bullets_; }

public://setter
	//エネミーマネージャーセット
	void SetEnemyManager(EnemyManager* _enemyManager) { enemyManager_ = _enemyManager; }
	//プレイヤーセット
	void SetPlayer(Player* _player) { player_ = _player; }

private:
	//砲弾の生成
	void CreateCannon();

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
	////銃弾のコンテナ
	//std::vector<std::unique_ptr<EnemyBullet>> bullets_;

};

