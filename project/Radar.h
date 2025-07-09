#pragma once
#include <Sprite.h>
#include <Vector2.h>
#include <array>
#include <cstdint>
#include <memory>

class Player;
class EnemyManager;
class Radar {
public:
	//初期化
	void Initialize();
	//更新
	void Update();

	//デバッグ用ImGui
	void DebugWithImGui();

private://非公開メンバ関数
	//レーダーにうつるエネミーの座標を更新
	void UpdateEnemyUIPosition();

public://setter
	void SetPlayer(Player* _player) { player_ = _player; }
	void SetEnemyManager(EnemyManager* _enemyManager) { enemyManager_ = _enemyManager; }

private://借用インスタンス
	Player* player_ = nullptr;
	EnemyManager* enemyManager_ = nullptr;

private://メンバ変数
	const Vector2 playerPos = { 640.0f,620.0f };	//プレイヤーのポジション(レーダー上)
	const float unitLength = 31.0f / 100.0f;		//レーダー上の1の長さ
	const float searchLength = 300.0f;		//索敵距離

	uint32_t thPlayerMark_ = 0u;
	std::unique_ptr<Sprite> playerMark_ = nullptr;

	static const int kEnemyUINum_ = 20;
	std::array<uint32_t, kEnemyUINum_> thEnemyMarks_;
	std::array<std::unique_ptr<Sprite>, kEnemyUINum_> enemyMarks_;


};

