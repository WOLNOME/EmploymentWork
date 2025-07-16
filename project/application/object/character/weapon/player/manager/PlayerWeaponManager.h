#pragma once
#include "JsonUtil.h"
#include <vector>
#include <memory>

//アプリケーション
#include "application/object/character/weapon/player/PlayerBullet.h"
#include "application/object/character/weapon/player/PlayerCannon.h"

class GameCamera;
class Player;
class PlayerWeaponManager {
public:
	//初期化
	void Initialize();
	//更新
	void Update();

	//デバッグ用パラメーター調整
	void DebugWithImGui();

public://getter
	//砲弾のコンテナ
	const std::vector<std::unique_ptr<PlayerCannon>>& GetCannons() const { return cannons_; }
	//銃弾のコンテナ
	const std::vector<std::unique_ptr<PlayerBullet>>& GetBullets() const { return bullets_; }

public://setter
	//カメラセット
	void SetGameCamera(GameCamera* _camera) { camera_ = _camera; };
	//プレイヤーセット
	void SetPlayer(Player* _player) { player_ = _player; }

private:
	//砲弾の生成
	void CreateCannon();
	//銃弾の生成
	void CreateBullet();

private:
	//カメラ
	GameCamera* camera_ = nullptr;
	//プレイヤー
	Player* player_ = nullptr;

private:
	//パラメーター
	json param_;

	//砲弾のコンテナ
	std::vector<std::unique_ptr<PlayerCannon>> cannons_;
	//銃弾のコンテナ
	std::vector<std::unique_ptr<PlayerBullet>> bullets_;

};

