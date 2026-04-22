#pragma once
#include <JsonUtil.h>
#include <string>
#include <cstdint>
#include <memory>

//アプリケーション
#include <application/system/tutorial/command/InputSystem.h>
#include <application/system/tutorial/command/CameraSystem.h>
#include <application/system/tutorial/command/MessageSystem.h>
#include <application/system/tutorial/command/ObjectiveSystem.h>
#include <application/system/tutorial/command/UISystem.h>
#include <application/system/tutorial/command/ObjectSystem.h>

// 前方宣言
class CameraManager;
class Player;
class PlayerWeaponManager;
class PlayerUI;
class ItemManager;
class EnemyManager;

/// <summary>
/// コマンド実行クラス
/// </summary>
class CommandExecutor {
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
	/// コマンドの実行
	/// </summary>
	/// <param name="_name">コマンド名</param>
	/// <param name="_param">コマンドパラメーター</param>
	/// <param name="_waitType">終了条件/param>
	void ExecuteCommand(const std::string& _name, const json& _param, const std::string& _waitType);

	/// ============================== ///
	///		getter
	/// ============================== ///

	 /// <summary>
	/// InputSystemの取得
	/// </summary>
	InputSystem* GetInputSystem() { return inputSystem_.get(); }
	/// <summary>
	/// CameraSystemの取得
	/// </summary>
	CameraSystem* GetCameraSystem() { return cameraSystem_.get(); }
	/// <summary>
	/// MessageSystemの取得
	/// </summary>
	MessageSystem* GetMessageSystem() { return messageSystem_.get(); }
	/// <summary>
	/// ObjectiveSystemの取得
	/// </summary>
	ObjectiveSystem* GetObjectiveSystem() { return objectiveSystem_.get(); }
	/// <summary>
	/// UISystemの取得
	/// </summary>
	UISystem* GetUISystem() { return uiSystem_.get(); }
	/// <summary>
	/// ObjectSystemの取得
	/// </summary>
	ObjectSystem* GetObjectSystem() { return objectSystem_.get(); }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// カメラマネージャーを設定します
	/// </summary>
	/// <param name="cameraManager">カメラマネージャーのインスタンス</param>
	void SetCameraManager(CameraManager* cameraManager);

	/// <summary>
	/// プレイヤーを設定します
	/// </summary>
	/// <param name="player">プレイヤーのインスタンス</param>
	void SetPlayer(Player* player);

	/// <summary>
	/// プレイヤー武器マネージャーを設定します
	/// </summary>
	/// <param name="playerWeaponManager">プレイヤー武器マネージャー</param>
	void SetPlayerWeaponManager(PlayerWeaponManager* playerWeaponManager);

	/// <summary>
	/// プレイヤーUIを設定します
	/// </summary>
	/// <param name="playerUI">プレイヤーUIのインスタンス</param>
	void SetPlayerUI(PlayerUI* playerUI);

	/// <summary>
	/// アイテムマネージャーを設定します
	/// </summary>
	/// <param name="itemManager">アイテムマネージャーのインスタンス</param>
	void SetItemManager(ItemManager* itemManager);

	/// <summary>
	/// エネミーマネージャーを設定します
	/// </summary>
	/// <param name="enemyManager">エネミーマネージャー</param>
	void SetEnemyManager(EnemyManager* enemyManager);

private:
	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	std::unique_ptr<InputSystem> inputSystem_;
	std::unique_ptr<CameraSystem> cameraSystem_;
	std::unique_ptr<MessageSystem> messageSystem_;
	std::unique_ptr<ObjectiveSystem> objectiveSystem_;
	std::unique_ptr<UISystem> uiSystem_;
	std::unique_ptr<ObjectSystem> objectSystem_;
};
