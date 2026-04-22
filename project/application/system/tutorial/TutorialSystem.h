#pragma once
#include <Audio.h>
#include <Sprite.h>
#include <memory>

//アプリケーション
#include <application/system/tutorial/manager/TutorialManager.h>
#include "application/object/level/loader/LevelLoader.h"
#include "application/object/character/player/Player.h"
#include "application/object/character/weapon/player/manager/PlayerWeaponManager.h"
#include "application/ui/player/PlayerUI.h"
#include "application/object/character/enemy/manager/EnemyManager.h"
#include "application/object/character/weapon/enemy/manager/EnemyWeaponManager.h"
#include "application/ui/enemy/EnemyUI.h"
#include <application/object/character/item/manager/ItemManager.h>
#include <application/ui/message/MessageUI.h>
#include <application/system/TimeScaleManager.h>
#include <application/system/tutorial/pause/Tuto_PauseSystem.h>

//前方宣言
class CameraManager;

/// <summary>
/// チュートリアルシーンのUIや演出、操作等を管理するクラス
/// </summary>
class TutorialSystem {
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
	///		setter
	/// ============================== ///

	/// <summary>
	/// カメラマネージャーのセット
	/// </summary>
	/// <param name="_cameraManager">カメラマネージャー</param>
	void SetCameraManager(CameraManager* _cameraManager);

private:

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//BGM
	std::unique_ptr<Norm::Audio> bgm_ = nullptr;

	//レベルローダー
	std::unique_ptr<LevelLoader> levelLoader_ = nullptr;

	//タイムスケールマネージャー
	std::unique_ptr<TimeScaleManager> timeScaleManager_ = nullptr;

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
	std::unique_ptr<Tuto_PauseSystem> pauseSystem_ = nullptr;



	//チュートリアルマネージャー
	std::unique_ptr<TutorialManager> tutorialManager_ = nullptr;



};

