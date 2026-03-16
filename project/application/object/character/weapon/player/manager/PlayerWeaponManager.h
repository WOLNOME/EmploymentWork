#pragma once
#include "JsonUtil.h"
#include <vector>
#include <memory>

//アプリケーション
#include "application/object/character/weapon/player/PlayerBullet.h"
#include "application/object/character/weapon/player/PlayerCannon.h"
#include "application/object/character/weapon/player/PlayerSpecial.h"

//前方宣言（アプリケーション）
class CameraManager;
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

	/// <summary>
	/// 砲弾のスポーン
	/// </summary>
	/// <param name="_initPos">初期位置</param>
	/// <param name="_initDirection">初期方向</param>
	void SpawnCannon(const Norm::Vector3& _initPos, const Norm::Vector3& _initDirection);
	/// <summary>
	/// 銃弾のスポーン
	/// </summary>
	/// <param name="_initPos">初期位置</param>
	/// <param name="_initDirection">初期方向</param>
	void SpawnBullet(const Norm::Vector3& _initPos, const Norm::Vector3& _initDirection);
	/// <summary>
	/// 必殺弾のスポーン
	/// </summary>
	/// <param name="_initPos">初期位置</param>
	/// <param name="_initDirection">初期方向</param>
	void SpawnSpecial(const Norm::Vector3& _initPos, const Norm::Vector3& _initDirection);

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
	/// <summary>
	/// 必殺弾のコンテナを取得する
	/// </summary>
	const std::vector<std::unique_ptr<PlayerSpecial>>& GetSpecials() const { return specials_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// カメラマネージャーを設定する
	/// </summary>
	/// <param name="_cameraManager">設定するカメラマネージャー</param>
	void SetCameraManager(CameraManager* _cameraManager);
	/// <summary>
	/// プレイヤーを設定する
	/// </summary>
	/// <param name="_player">設定するプレイヤー</param>
	void SetPlayer(Player* _player) { player_ = _player; }

private:
	/// ============================== ///
	///		メンバ関数（private）
	/// ============================== ///

	/// <summary>
	/// スポーンの共通処理(Spawn処理は爆弾のみ共通ではないので、関数オブジェクトを別で渡している)
	/// </summary>
	/// <typeparam name="T">プール内に格納されているオブジェクトの型</typeparam>
	/// <typeparam name="SpawnFunc">Spawn処理を行う関数オブジェクト</typeparam>
	/// <param name="container">コンテナ</param>
	/// <param name="spawnFunc">実際のSpawn処理を定義する関数オブジェクト</param>
	template<class T, class SpawnFunc>
	void SpawnFromPool(
		std::vector<std::unique_ptr<T>>& container,
		SpawnFunc spawnFunc) {
		for (auto& obj : container) {
			if (obj->GetState() != BaseCharacter::State::kIdle)
				continue;

			spawnFunc(obj.get());
			break;
		}
	}

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//カメラ
	CameraManager* cameraManager_ = nullptr;
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
	//必殺弾のコンテナ
	std::vector<std::unique_ptr<PlayerSpecial>> specials_;

};

