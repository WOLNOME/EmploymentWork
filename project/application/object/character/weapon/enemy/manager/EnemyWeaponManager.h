#pragma once
#include "JsonUtil.h"
#include <vector>
#include <memory>

//アプリケーション
#include "application/object/character/weapon/enemy/EnemyCannon.h"
#include "application/object/character/weapon/enemy/EnemyBullet.h"
#include "application/object/character/weapon/enemy/EnemyBomb.h"


//前方宣言（アプリケーション）
class EnemyManager;
class Player;
class PlayerUI;

/// <summary>
/// 敵の使う武器全般の管理を行うクラス
/// </summary>
class EnemyWeaponManager {
public:
	/// ============================== ///
	///		メンバ関数（public）
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
	/// <param name="_targetPos">目標位置</param>
	void SpawnCannon(const Norm::Vector3& _initPos, const Norm::Vector3& _targetPos);
	/// <summary>
	/// 銃弾のスポーン
	/// </summary>
	/// <param name="_initPos">初期位置</param>
	/// <param name="_targetPos">目標位置</param>
	void SpawnBullet(const Norm::Vector3& _initPos, const Norm::Vector3& _targetPos);
	/// <summary>
	/// 爆弾のスポーン
	/// </summary>
	/// <param name="_bombMethod">爆弾の撃ち方</param>
	/// <param name="_initPos">初期位置</param>
	/// <param name="_targetPos">目標位置</param>
	void SpawnBomb(const BombMethod& _bombMethod, const Norm::Vector3& _initPos, const Norm::Vector3& _targetPos);

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// 砲弾のコンテナを取得する
	/// </summary>
	/// <returns>砲弾のコンテナ</returns>
	const std::vector<std::unique_ptr<EnemyCannon>>& GetCannons() const { return cannons_; }
	/// <summary>
	/// 銃弾のコンテナを取得する
	/// </summary>
	/// <returns>銃弾のコンテナ</returns>
	const std::vector<std::unique_ptr<EnemyBullet>>& GetBullets() const { return bullets_; }
	/// <summary>
	/// 爆弾のコンテナを取得する
	/// </summary>
	/// <returns>爆弾のコンテナ</returns>
	const std::vector<std::unique_ptr<EnemyBomb>>& GetBombs() const { return bombs_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// エネミーマネージャーを設定する
	/// </summary>
	/// <param name="_enemyManager">設定するエネミーマネージャー</param>
	void SetEnemyManager(EnemyManager* _enemyManager) { enemyManager_ = _enemyManager; }
	/// <summary>
	/// プレイヤーを設定する
	/// </summary>
	/// <param name="_player">設定するプレイヤー</param>
	void SetPlayer(Player* _player) { player_ = _player; }
	/// <summary>
	/// プレイヤーUIを設定する
	/// </summary>
	/// <param name="_playerUI">設定するプレイヤーUI</param>
	void SetPlayerUI(PlayerUI* _playerUI);

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

	//エネミーマネージャー
	EnemyManager* enemyManager_ = nullptr;
	//プレイヤー
	Player* player_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json param_;

	//砲弾のコンテナ
	std::vector<std::unique_ptr<EnemyCannon>> cannons_;
	//銃弾のコンテナ
	std::vector<std::unique_ptr<EnemyBullet>> bullets_;
	//爆弾のコンテナ
	std::vector<std::unique_ptr<EnemyBomb>> bombs_;


};

