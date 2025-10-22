#pragma once
#include "JsonUtil.h"
#include <vector>
#include <memory>

//アプリケーション
#include "application/object/character/weapon/player/PlayerBullet.h"
#include "application/object/character/weapon/player/PlayerCannon.h"

class GameCamera;
class Player;

/// <summary>
/// プレイヤーの武器（砲弾・銃弾）の管理を行うクラスです。
/// </summary>
class PlayerWeaponManager {
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
	/// 砲弾のコンテナを取得する
	/// </summary>
	const std::vector<std::unique_ptr<PlayerCannon>>& GetCannons() const { return cannons_; }
	/// <summary>
	/// 銃弾のコンテナを取得する
	/// </summary>
	const std::vector<std::unique_ptr<PlayerBullet>>& GetBullets() const { return bullets_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// ゲームカメラを設定する
	/// </summary>
	/// <param name="_camera">設定するゲームカメラ</param>
	void SetGameCamera(GameCamera* _camera) { camera_ = _camera; };
	/// <summary>
	/// プレイヤーを設定する
	/// </summary>
	/// <param name="_player">設定するプレイヤー</param>
	void SetPlayer(Player* _player) { player_ = _player; }

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// 砲弾を生成する
	/// </summary>
	void CreateCannon();
	/// <summary>
	/// 銃弾を生成する
	/// </summary>
	void CreateBullet();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//カメラ
	GameCamera* camera_ = nullptr;
	//プレイヤー
	Player* player_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json param_;

	//砲弾のコンテナ
	std::vector<std::unique_ptr<PlayerCannon>> cannons_;
	//銃弾のコンテナ
	std::vector<std::unique_ptr<PlayerBullet>> bullets_;

};

