#pragma once
#include "GameCamera.h"
#include "Sprite.h"
#include "JsonUtil.h"
#include <array>
#include <cstdint>

class EnemyManager;
class EnemyUI {
public:
	//初期化
	void Initialize();
	//更新
	void Update();

	//デバッグ用ImGui
	void DebugWithImGui();

public://setter
	void SetEnemyManager(EnemyManager* _enemyManager) { enemyManager_ = _enemyManager; }
	void SetGameCamera(GameCamera* _camera) { camera_ = _camera; }
private:
	//エネミー
	EnemyManager* enemyManager_ = nullptr;
	//カメラ
	GameCamera* camera_ = nullptr;
private:
	//HPバー
	static const int kNumHPBar = 10; //一度に表示可能なHPバーの数
	std::array<uint32_t, 2> thHPBars_;
	std::array<std::array<std::unique_ptr<Sprite>, 2>, kNumHPBar> spriteHPBar_;
	std::array<bool, kNumHPBar> isHPBarVisible_; //各HPバーの表示状態 
	float hpBarWidth_ = 0.0f;
	float hpBarHeight_ = 0.0f;

private://データ駆動設計用パラメータ
	json param_;

};

