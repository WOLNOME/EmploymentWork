#pragma once
#include "GameCamera.h"
#include "SceneLight.h"
#include "JsonUtil.h"
#include <vector>
#include <memory>

//アプリケーション
#include "application/object/player/weapon/PlayerBullet.h"
#include "application/object/player/weapon/PlayerCannon.h"

class Player;
class PlayerWeaponManager {
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
	//線描画
	void DrawLine();

	//デバッグ用パラメーター調整
	void DebugWithImGui();

public://getter
	//砲弾のコンテナ
	const std::vector<std::unique_ptr<PlayerCannon>>& GetCannons() const { return cannons_; }
	//銃弾のコンテナ
	const std::vector<std::unique_ptr<PlayerBullet>>& GetBullets() const { return bullets_; }

public://setter
	//カメラセット
	void SetCamera(GameCamera* _camera);
	//ライトセット
	void SetLight(SceneLight* _light);
	//プレイヤーセット
	void SetPlayer(Player* _player) { player_ = _player; }

private:
	//砲弾の生成
	void CreateCannon();

private:
	//カメラ
	GameCamera* camera_ = nullptr;
	//シーンライト
	SceneLight* sceneLight_ = nullptr;
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

