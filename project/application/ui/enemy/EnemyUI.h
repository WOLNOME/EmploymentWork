#pragma once
#include "JsonUtil.h"

//アプリケーション
#include "detail/EnemyHPUI.h"

//前方宣言（エンジン）
namespace Norm {
	class GameCamera;
}
//前方宣言（アプリケーション）
class EnemyManager;


/// <summary>
/// 敵に関するUI全てを管理するクラス
/// </summary>
class EnemyUI {
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
	/// デバッグ用ImGui
	/// </summary>
	void DebugWithImGui();

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// エネミーマネージャーのセット
	/// </summary>
	/// <param name="_enemyManager">エネミーマネージャー</param>
	void SetEnemyManager(EnemyManager* _enemyManager);
	/// <summary>
	/// ゲームカメラのセット
	/// </summary>
	/// <param name="_camera">ゲームカメラ</param>
	void SetGameCamera(Norm::GameCamera* _camera);
private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//HPUI
	std::unique_ptr<EnemyHPUI> enemyHPUI_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	json param_;

};

