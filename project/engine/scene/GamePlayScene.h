#pragma once
#include "BaseScene.h"
#include "GameCamera.h"
#include "application/object/Skydome.h"
#include "application/object/Ground.h"
#include "application/object/player/Player.h"
#include "application/PlayerWeaponManager.h"
#include "application/object/player/ui/PlayerUI.h"
#include "application/EnemyManager.h"
#include "application/object/enemy/ui/EnemyUI.h"


class GamePlayScene : public BaseScene {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 終了時
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:
	//インプット
	Input* input_ = nullptr;

	//ゲーム用カメラ
	std::unique_ptr<GameCamera> camera_ = nullptr;
private://ライト
	//平行光源
	std::unique_ptr<DirectionalLight> dirLight_;
private://オブジェクト
	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//地面
	std::unique_ptr<Ground> ground_ = nullptr;

	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	//プレイヤー武器マネージャー
	std::unique_ptr<PlayerWeaponManager> playerWeaponManager_ = nullptr;
	//敵管理マネージャー
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;

private://UI
	//プレイヤーUI
	std::unique_ptr<PlayerUI> playerUI_ = nullptr;
	//敵UI
	std::unique_ptr<EnemyUI> enemyUI_ = nullptr;

};

