#pragma once
#include "BaseScene.h"
#include "GameCamera.h"

//アプリケーション
#include "application/object/environment/Skydome.h"
#include "application/object/environment/Ground.h"
#include "application/object/character/player/Player.h"
#include "application/object/character/weapon/player/manager/PlayerWeaponManager.h"
#include "application/ui/player/PlayerUI.h"
#include "application/object/character/enemy/manager/EnemyManager.h"
#include "application/object/character/weapon/enemy/manager/EnemyWeaponManager.h"
#include "application/ui/enemy/EnemyUI.h"
#include <application/object/character/item/manager/ItemManager.h>
#include <application/ui/message/MessageUI.h>

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
	/// デバッグ処理
	/// </summary>
	void DebugWithImGui() override;

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
	//レベルローダー
	std::unique_ptr<LevelLoader> levelLoader_ = nullptr;

	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	//プレイヤー武器マネージャー
	std::unique_ptr<PlayerWeaponManager> playerWeaponManager_ = nullptr;
	//敵管理マネージャー
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;
	//敵武器マネージャー
	std::unique_ptr<EnemyWeaponManager> enemyWeaponManager_ = nullptr;

	//アイテムマネージャー
	std::unique_ptr<ItemManager> itemManager_ = nullptr;

private://UI
	//プレイヤーUI
	std::unique_ptr<PlayerUI> playerUI_ = nullptr;
	//敵UI
	std::unique_ptr<EnemyUI> enemyUI_ = nullptr;
	//メッセージUI
	std::unique_ptr<MessageUI> messageUI_ = nullptr;

};

