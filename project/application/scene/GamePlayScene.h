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
#include <application/system/StartDirection.h>
#include <application/system/EndDirection.h>
#include <application/system/TimeScaleManager.h>
#include <application/system/PauseSystem.h>

/// <summary>
/// ゲームプレイシーン全般を管理するクラス
/// </summary>
class GamePlayScene : public BaseScene {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

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
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//インプット
	Input* input_ = nullptr;

	//ゲーム用カメラ
	std::unique_ptr<GameCamera> camera_ = nullptr;

	//タイムスケールマネージャー
	std::unique_ptr<TimeScaleManager> timeScaleManager_ = nullptr;

	//スタート演出
	std::unique_ptr<StartDirection> startDirection_ = nullptr;

	//エンド演出
	std::unique_ptr<EndDirection> endDirection_ = nullptr;

	//平行光源
	std::unique_ptr<DirectionalLight> dirLight_;

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

	//プレイヤーUI
	std::unique_ptr<PlayerUI> playerUI_ = nullptr;
	//敵UI
	std::unique_ptr<EnemyUI> enemyUI_ = nullptr;
	//メッセージUI
	std::unique_ptr<MessageUI> messageUI_ = nullptr;

	//ポーズ
	std::unique_ptr<PauseSystem> pauseSystem_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//デバッグ用フラグ
	bool isDebug_ = false;

};

