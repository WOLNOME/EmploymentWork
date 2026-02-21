#pragma once
#include <GameCamera.h>
#include <unordered_map>

/// <summary>
/// カメラマネージャー（アクティブカメラや切り替えを担当）
/// </summary>
class CameraManager {
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
	/// デバッグ処理 
	/// </summary>
	void DebugWithImGui();

	/// <summary>
	/// カメラの登録
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="camera">カメラのユニークポインタ</param>
	void RegistCamera(const std::string& name, std::unique_ptr<Norm::GameCamera> camera);

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// アクティブカメラのセット
	/// </summary>
	/// <param name="name">名前</param>
	void SetActiveCamera(const std::string& name);

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// アクティブカメラの取得
	/// </summary>
	/// <returns>アクティブカメラのポインタ</returns>
	Norm::GameCamera* GetActiveCamera() { return activeCamera_; }

private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	std::unordered_map<std::string, std::unique_ptr<Norm::GameCamera>> cameras_;
	Norm::GameCamera* activeCamera_ = nullptr;

};

