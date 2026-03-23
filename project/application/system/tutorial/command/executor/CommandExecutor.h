#pragma once
#include <JsonUtil.h>
#include <string>
#include <cstdint>
#include <memory>

// 前方宣言
class InputSystem;
class CameraSystem;
class MessageSystem;
class ObjectiveSystem;
class UISystem;
class ObjectSystem;

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
	/// コマンドの実行
	/// </summary>
	/// <param name="_name">コマンド名</param>
	/// <param name="_param">コマンドパラメーター</param>
	void ExecuteCommand(const std::string& _name, const json& _param);

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
