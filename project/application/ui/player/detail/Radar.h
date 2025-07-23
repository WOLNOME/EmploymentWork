#pragma once
#include <Sprite.h>
#include <Vector2.h>
#include <Vector4.h>
#include <array>
#include <cstdint>
#include <memory>

class BaseCamera;
class Player;
class EnemyManager;
class ItemManager;
class Radar {
public:
	//初期化
	void Initialize();
	//更新
	void Update();

	//シェイクの適用
	void AttachShake(const Vector2& _shakeOffset);
	//点滅の適用
	void AttachBlinking(const Vector4& _color);

	//デバッグ用ImGui
	void DebugWithImGui();

private://非公開メンバ関数
	//エネミーマークを更新
	void UpdateEnemyMark();
	//アイテムマークを更新
	void UpdateItemMark();
	//コンパスを更新
	void UpdateCompass();

public://setter
	void SetGameCamera(BaseCamera* _caemra) { camera_ = _caemra; }
	void SetPlayer(Player* _player) { player_ = _player; }
	void SetEnemyManager(EnemyManager* _enemyManager) { enemyManager_ = _enemyManager; }
	void SetItemManager(ItemManager* _itemManager) { itemManager_ = _itemManager; }

private://借用インスタンス
	BaseCamera* camera_ = nullptr;
	Player* player_ = nullptr;
	EnemyManager* enemyManager_ = nullptr;
	ItemManager* itemManager_ = nullptr;

private://メンバ変数
	const Vector2 centerPosition = { 640.0f,620.0f };	//レーダーの中心座標
	const float unitLength = 31.0f / 100.0f;		//レーダー上の1の長さ
	const float searchLength = 300.0f;		//索敵距離

	uint32_t thPlayerMark_ = 0u;
	std::unique_ptr<Sprite> playerMark_ = nullptr;

	static const int kEnemyUINum_ = 20;
	uint32_t thEnemyMark_ = 0u;
	std::array<std::unique_ptr<Sprite>, kEnemyUINum_> enemyMarks_;

	static const int kItemUINum_ = 10;
	uint32_t thItemMark_ = 0u;
	std::array<std::unique_ptr<Sprite>, kItemUINum_> itemMarks_;

	uint32_t thCompass_ = 0u;
	std::unique_ptr<Sprite> compass_ = nullptr;


};

