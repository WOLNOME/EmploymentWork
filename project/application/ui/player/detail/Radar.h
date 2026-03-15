#pragma once
#include <JsonUtil.h>
#include <Sprite.h>
#include <Vector2.h>
#include <Vector4.h>
#include <array>
#include <cstdint>
#include <memory>

//前方宣言（アプリケーション）
class CameraManager;
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
	void AttachShake(const Norm::Vector2& _shakeOffset);
	/// <summary>
	/// 点滅の適用
	/// </summary>
	/// <param name="_color">色</param>
	void AttachBlinking(const Norm::Vector4& _color);

	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui();

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// カメラマネージャーを設定する
	/// </summary>
	/// <param name="_cameraManager">カメラマネージャー</param>
	void SetCameraManager(CameraManager* _cameraManager) { cameraManager_ = _cameraManager; }
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
	void UpdatePlayerMark();
	/// <summary>
	/// エネミーマークを更新
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

	CameraManager* cameraManager_ = nullptr;
	Player* player_ = nullptr;
	EnemyManager* enemyManager_ = nullptr;
	ItemManager* itemManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメータ
	json param_;

	Norm::Vector2 centerPosition_;	//レーダーの中心座標
	const float kUnitLength_ = 31.0f / 100.0f;		//レーダー上の100mの長さ
	const float kSearchLength_ = 300.0f;		//索敵距離

	//コンパス
	uint32_t thCompass_ = 0u;
	std::unique_ptr<Norm::Sprite> compass_ = nullptr;
	//レーダー基盤
	uint32_t thRadarBase_ = 0u;
	std::unique_ptr<Norm::Sprite> radarBase_ = nullptr;

	//テクスチャハンドル
	uint32_t thCharacterMark_ = 0u;
	uint32_t thPlayerMark_ = 0u;
	uint32_t thItemMark_ = 0u;

	//プレイヤーマーク
	std::unique_ptr<Norm::Sprite> playerMark_ = nullptr;
	//敵マーク
	static const int kEnemyUINum_ = 20;
	std::array<std::unique_ptr<Norm::Sprite>, kEnemyUINum_> enemyMarks_;
	//アイテムマーク
	static const int kItemUINum_ = 10;
	std::array<std::unique_ptr<Norm::Sprite>, kItemUINum_> itemMarks_;

};

