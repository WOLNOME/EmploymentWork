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

/// <summary>
/// レーダーUIを管理するクラス
/// </summary>
class Radar {
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
	/// シェイクの適用
	/// </summary>
	/// <param name="_shakeOffset">シェイクオフセット</param>
	void AttachShake(const Vector2& _shakeOffset);
	/// <summary>
	/// 点滅の適用
	/// </summary>
	/// <param name="_color">色</param>
	void AttachBlinking(const Vector4& _color);

	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui();

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// ゲームカメラを設定する
	/// </summary>
	/// <param name="_caemra">ゲームカメラ</param>
	void SetGameCamera(BaseCamera* _caemra) { camera_ = _caemra; }
	/// <summary>
	/// プレイヤーを設定する
	/// </summary>
	/// <param name="_player">プレイヤー</param>
	void SetPlayer(Player* _player) { player_ = _player; }
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


private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// プレイヤーマークを更新
	/// </summary>
	void UpdateEnemyMark();
	/// <summary>
	/// アイテムマークを更新
	/// </summary>
	void UpdateItemMark();
	/// <summary>
	/// コンパスを更新
	/// </summary>
	void UpdateCompass();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	BaseCamera* camera_ = nullptr;
	Player* player_ = nullptr;
	EnemyManager* enemyManager_ = nullptr;
	ItemManager* itemManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

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

