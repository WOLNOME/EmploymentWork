#pragma once
#include <JsonUtil.h>
//アプリケーション
#include "detail/ReticleUI.h"
#include "detail/DecorativeUI.h"
#include "detail/PlayerHPUI.h"
#include "detail/WeaponUI.h"
#include "detail/Radar.h"
#include "detail/HitIndicator.h"
#include "detail/ItemUI.h"
#include "detail/OperationUI.h"
#include "detail/VelocityUI.h"
#include "detail/KeyUI.h"
#include "detail/DirectionUI.h"
#include "detail/DamageUI.h"

//前方宣言（エンジン）
namespace Norm {
	class GameCamera;
}
//前方宣言（アプリケーション）
class Player;
class EnemyManager;
class ItemManager;

/// <summary>
/// プレイヤーに関するUI全てを管理するクラス
/// </summary>
class PlayerUI {
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
	///		getter
	/// ============================== ///

	/// <summary>
	/// 被弾インジケーターの取得
	/// </summary>
	/// <returns>被弾インジケーター</returns>
	HitIndicator* GetHitIndicator() { return hitIndicator_.get(); }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// プレイヤーの設定
	/// </summary>
	/// <param name="_player">プレイヤー</param>
	void SetPlayer(Player* _player);
	/// <summary>
	/// エネミーマネージャーの設定
	/// </summary>
	/// <param name="_enemyManager">エネミーマネージャー</param>
	void SetEnemyManager(EnemyManager* _enemyManager);
	/// <summary>
	/// アイテムマネージャーの設定
	/// </summary>
	/// <param name="_itemManager">アイテムマネージャー</param>
	void SetItemManager(ItemManager* _itemManager);
	/// <summary>
	/// ゲームカメラの設定
	/// </summary>
	/// <param name="_camera">ゲームカメラ</param>
	void SetGameCamera(Norm::GameCamera* _camera);

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// シェイク処理
	/// </summary>
	void DamageShaking();
	/// <summary>
	/// 点滅処理
	/// </summary>
	void DamageBlinking();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//プレイヤー
	Player* player_ = nullptr;
	//カメラ
	Norm::GameCamera* camera_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//レティクルUI
	std::unique_ptr<ReticleUI> reticleUI_ = nullptr;
	//装飾用UI
	std::unique_ptr<DecorativeUI> decorativeUI_ = nullptr;
	//HPバー
	std::unique_ptr<PlayerHPUI> playerHPUI_ = nullptr;
	//武器UI
	std::unique_ptr<WeaponUI> weaponUI_ = nullptr;
	//レーダーUI
	std::unique_ptr<Radar> radar_ = nullptr;
	//被弾インジケーターUI
	std::unique_ptr<HitIndicator> hitIndicator_ = nullptr;
	//アイテムUI
	std::unique_ptr<ItemUI> itemUI_ = nullptr;
	//操作UI
	std::unique_ptr<OperationUI> operationUI_ = nullptr;
	//速度UI
	std::unique_ptr<VelocityUI> velocityUI_ = nullptr;
	//キーUI
	std::unique_ptr<KeyUI> keyUI_ = nullptr;
	//方向UI
	std::unique_ptr<DirectionUI> directionUI_ = nullptr;
	//ダメージUI
	std::unique_ptr<DamageUI> damageUI_ = nullptr;

	//パラメーター
	json param_;

	bool isDamage_ = false;
	bool isBright_ = true;
	float blinkTimer_ = 0.0f;

};

