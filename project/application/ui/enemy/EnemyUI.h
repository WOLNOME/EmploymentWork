#pragma once
#include "JsonUtil.h"

//アプリケーション
#include "detail/EnemyHPUI.h"
#include "detail/EnemyReactionUI.h"

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
	///		getter
	/// ============================== ///

	/// <summary>
	/// リアクションUIを取得
	/// </summary>
	/// <returns>リアクションUIのポインタ</returns>
	EnemyReactionUI* GetEnemyReactionUI() { return enemyReactionUI_.get(); }

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
	//リアクションUI
	std::unique_ptr<EnemyReactionUI> enemyReactionUI_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json param_;

};

