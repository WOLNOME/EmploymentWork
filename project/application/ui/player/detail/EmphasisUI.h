#pragma once
#include <JsonUtil.h>
#include <Sprite.h>
#include <array>
#include <cstdint>
#include <memory>

//前方宣言（アプリケーション）
class CameraManager;
class EnemyManager;
class ItemManager;

/// <summary>
/// 強調UIを管理するクラス
/// </summary>
class EmphasisUI {
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
	/// デバッグ
	/// </summary>
	void Debug();

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// カメラマネージャーを設定する
	/// </summary>
	/// <param name="_cameraManager">カメラマネージャー</param>
	void SetCameraManager(CameraManager* _cameraManager) { cameraManager_ = _cameraManager; }
	/// <summary>
	/// エネミーマネージャーを設定する
	/// </summary>
	/// <param name="_enemyManager">エネミーマネージャー</param>
	void SetEnemyManager(EnemyManager* _enemyManager) { enemyManager_ = _enemyManager; }
	/// <summary>
	/// アイテムマネージャーを設定する
	/// </summary>
	/// <param name="_itemManager">アイテムマネージャー</param>
	void SetItemManager(ItemManager* _itemManager) { itemManager_ = _itemManager; }

	/// <summary>
	/// 表示するかを設定
	/// </summary>
	/// <param name="_isDisplay">表示するか</param>
	void SetIsDisplay(bool _isDisplay);


private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// 強調アイコンの更新処理
	/// </summary>
	void UpdateIcon();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	CameraManager* cameraManager_ = nullptr;
	EnemyManager* enemyManager_ = nullptr;
	ItemManager* itemManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメータ
	json param_;

	//強調アイコン
	static const int kIconNum_ = 10;
	uint32_t thIcon_ = 0u;
	std::array<std::unique_ptr<Norm::Sprite>, kIconNum_> icons_;


};

