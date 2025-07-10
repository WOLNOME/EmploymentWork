#pragma once
#include <Sprite.h>
#include <JsonUtil.h>

#include <array>
#include <cstdint>

//アプリケーション
#include <application/ui/player/detail/Radar.h>

class Player;
class EnemyManager;
class GameCamera;
class PlayerUI {
public:
	//初期化
	void Initialize();
	//更新
	void Update();

	//デバッグ用ImGui
	void DebugWithImGui();

public://setter
	void SetPlayer(Player* _player);
	void SetEnemyManager(EnemyManager* _enemyManager);
	void SetCamera(GameCamera* _camera);

private://非公開メンバ関数
	//点滅処理
	void DamageBlinking();

private:
	//プレイヤー
	Player* player_ = nullptr;
	//エネミーマネージャー
	EnemyManager* enemyManager_ = nullptr;
	//カメラ
	GameCamera* camera_ = nullptr;
private:
	//2dレティクル
	uint32_t th2dReticle_ = 0u;
	std::unique_ptr<Sprite> sprite2dReticle_ = nullptr;
	//FPSUI
	uint32_t thFPSUI_ = 0u;
	std::unique_ptr<Sprite> spriteFPSUI_ = nullptr;
	//HPバー
	std::array<uint32_t, 2> thHPBar_;
	std::array<std::unique_ptr<Sprite>, 2> spriteHPBar_;
	//砲弾UI
	std::array<uint32_t, 2> thCannon_;
	std::array<std::unique_ptr<Sprite>, 2> spriteCannon_;
	//銃弾UI
	std::array<uint32_t, 2> thBullet_;
	std::array<std::unique_ptr<Sprite>, 2> spriteBullet_;
	//レーダーUI
	std::unique_ptr<Radar> radar_ = nullptr;


private://メンバ変数
	//パラメーター
	json param_;

	bool isDamage = false;
	bool isBright = true;
	float blinkTimer = 0.0f;


};

