#pragma once
#include <JsonUtil.h>
#include <Sprite.h>
#include <array>
#include <memory>

class GameCamera;
class EnemyManager;
class EnemyHPUI {
public:
	//初期化
	void Initialize();
	//更新
	void Update();

public://setter
	void SetEnemyManager(EnemyManager* _enemyManager) { enemyManager_ = _enemyManager; }
	void SetGameCamera(GameCamera* _camera) { camera_ = _camera; }
private:
	//エネミー
	EnemyManager* enemyManager_ = nullptr;
	//カメラ
	GameCamera* camera_ = nullptr;
private:
	//パラメーター
	json param_;

	//HPバー
	static const int kNumHPBar = 10; //一度に表示可能なHPバーの数
	std::array<uint32_t, 2> textureHandles_;
	std::array<std::array<std::unique_ptr<Sprite>, 2>, kNumHPBar> sprites_;
	std::array<bool, kNumHPBar> isHPBarVisible_; //各HPバーの表示状態 
	float hpBarWidth_ = 0.0f;
	float hpBarHeight_ = 0.0f;

};

