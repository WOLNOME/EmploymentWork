#pragma once
#include "JsonUtil.h"

//アプリケーション
#include "detail/EnemyHPUI.h"
#include "detail/EnemyReactionUI.h"
#include "detail/EnemyInformationUI.h"

//前方宣言（アプリケーション）
class CameraManager;
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
	/// カメラマネージャーのセット
	/// </summary>
	/// <param name="_cameraManager">カメラマネージャー</param>
	void SetCameraManager(CameraManager* _cameraManager);

private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//HPUI
	std::unique_ptr<EnemyHPUI> enemyHPUI_ = nullptr;
	//リアクションUI
	std::unique_ptr<EnemyReactionUI> enemyReactionUI_ = nullptr;
	//情報UI
	std::unique_ptr<EnemyInformationUI> enemyInformationUI_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json param_;

};

