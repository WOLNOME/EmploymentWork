#pragma once
#include <JsonUtil.h>
//アプリケーション
#include "detail/ReticleUI.h"
#include "detail/DecorativeUI.h"
#include "detail/PlayerHPUI.h"
#include "detail/CannonUI.h"
#include "detail/BulletUI.h"
#include "detail/Radar.h"
#include "detail/HitIndicator.h"
#include "detail/ItemUI.h"

class Player;
class EnemyManager;
class ItemManager;
class GameCamera;

/// <summary>
/// プレイヤーに関するUI全てを管理するクラス
/// </summary>
class PlayerUI {
public:
	//初期化
	void Initialize();
	//更新
	void Update();

	//デバッグ用ImGui
	void DebugWithImGui();

public://getter
	HitIndicator* GetHitIndicator() { return hitIndicator_.get(); }

public://setter
	void SetPlayer(Player* _player);
	void SetEnemyManager(EnemyManager* _enemyManager);
	void SetItemManager(ItemManager* _itemManager);
	void SetGameCamera(GameCamera* _camera);

private://非公開メンバ関数
	//シェイク処理
	void DamageShaking();
	//点滅処理
	void DamageBlinking();

private://借用インスタンス
	//プレイヤー
	Player* player_ = nullptr;
	//カメラ
	GameCamera* camera_ = nullptr;
private:
	//レティクルUI
	std::unique_ptr<ReticleUI> reticleUI_ = nullptr;
	//装飾用UI
	std::unique_ptr<DecorativeUI> decorativeUI_ = nullptr;
	//HPバー
	std::unique_ptr<PlayerHPUI> playerHPUI_ = nullptr;
	//砲弾UI
	std::unique_ptr<CannonUI> cannonUI_ = nullptr;
	//銃弾UI
	std::unique_ptr<BulletUI> bulletUI_ = nullptr;
	//レーダーUI
	std::unique_ptr<Radar> radar_ = nullptr;
	//被弾インジケーターUI
	std::unique_ptr<HitIndicator> hitIndicator_ = nullptr;
	//アイテムUI
	std::unique_ptr<ItemUI> itemUI_ = nullptr;

private://メンバ変数
	//パラメーター
	json param_;

	bool isDamage = false;
	bool isBright = true;
	float blinkTimer = 0.0f;


};

