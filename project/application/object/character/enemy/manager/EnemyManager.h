#pragma once
#include "JsonUtil.h"
#include <vector>
#include <memory>

//アプリケーション
#include "application/object/character/enemy/tank/Canota.h"
#include "application/object/character/enemy/tank/KeyCanota.h"
#include "application/object/character/enemy/jet/Jet.h"
#include "application/object/character/enemy/boss/Boss.h"

//前方宣言（アプリケーション）
class LevelLoader;
class Player;
class ItemManager;
class EnemyWeaponManager;
class MessageUI;
class EnemyUI;
class CameraManager;

/// <summary>
/// エネミー全体の管理を行うクラス
/// </summary>
class EnemyManager {
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
	/// キャノ太のスポーン
	/// </summary>
	/// <param name="_initPos">初期位置</param>
	/// <param name="_initRotate">初期回転</param>
	void CanotaSpawn(const Norm::Vector3& _initPos, const Norm::Vector3& _initRotate);

	/// <summary>
	/// キーキャノ太のスポーン
	/// </summary>
	/// <param name="_initPos">初期位置</param>
	/// <param name="_initRotate">初期回転</param>
	void KeyCanotaSpawn(const Norm::Vector3& _initPos, const Norm::Vector3& _initRotate);

	/// <summary>
	/// ジェットのスポーン
	/// </summary>
	/// <param name="_initPos">初期位置</param>
	/// <param name="_initRotate">初期回転</param>
	void JetSpawn(const Norm::Vector3& _initPos, const Norm::Vector3& _initRotate);

	/// <summary>
	/// ボスのスポーン
	/// </summary>
	/// <param name="_initPos">初期位置</param>
	/// <param name="_initRotate">初期回転</param>
	void BossSpawn(const Norm::Vector3& _initPos, const Norm::Vector3& _initRotate);

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// キャノ太のコンテナ取得
	/// </summary>
	/// <returns>キャノ太のコンテナ</returns>
	const std::vector<std::unique_ptr<Canota>>& GetCanotas() const { return canotas_; }
	/// <summary>
	/// キーキャノ太のコンテナ取得
	/// </summary>
	/// <returns>キーキャノ太のコンテナ</returns>
	const std::vector<std::unique_ptr<KeyCanota>>& GetKeyCanotas() const { return keyCanotas_; }
	/// <summary>
	/// ジェットのコンテナ取得
	/// </summary>
	/// <returns>ジェットのコンテナ</returns>
	const std::vector<std::unique_ptr<Jet>>& GetJets() const { return jets_; }
	/// <summary>
	/// ボスのコンテナ取得
	/// </summary>
	/// <returns></returns>
	const std::vector<std::unique_ptr<Boss>>& GetBosses() const { return bosses_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// レベルローダーのセット
	/// </summary>
	/// <param name="_levelLoader">レベルローダーのポインタ</param>
	void SetLevelLoader(LevelLoader* _levelLoader);
	/// <summary>
	/// プレイヤーのセット
	/// </summary>
	/// <param name="_player">プレイヤーのポインタ</param>
	void SetPlayer(Player* _player);
	/// <summary>
	/// アイテムマネージャーのセット
	/// </summary>
	/// <param name="_itemManager">アイテムマネージャーのポインタ</param>
	void SetItemManager(ItemManager* _itemManager);
	/// <summary>
	/// 敵武器マネージャーのセット
	/// </summary>
	/// <param name="_enemyWeaponManager">敵武器マネージャーのポインタ</param>
	void SetEnemyWeaponManager(EnemyWeaponManager* _enemyWeaponManager);
	/// <summary>
	/// メッセージUIのセット
	/// </summary>
	/// <param name="_messageUI">メッセージUIのポインタ</param>
	void SetMessageUI(MessageUI* _messageUI);
	/// <summary>
	/// 敵UIのセット
	/// </summary>
	/// <param name="_enemyUI"></param>
	void SetEnemyUI(EnemyUI* _enemyUI);
	/// <summary>
	/// カメラマネージャーのセット
	/// </summary>
	/// <param name="_cameraManager">カメラマネージャー</param>
	void SetCameraManager(CameraManager* _cameraManager);

	/// <summary>
	/// アクティブかどうかのセット
	/// </summary>
	/// <param name="isActive">アクティブかどうか</param>
	void SetIsActive(bool isActive);

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

	//プレイヤー
	Player* player_ = nullptr;
	//アイテムマネージャー
	ItemManager* itemManager_ = nullptr;
	//敵武器マネージャー
	EnemyWeaponManager* enemyWeaponManager_ = nullptr;
	//敵UI
	EnemyUI* enemyUI_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json param_;

	//キャノ太のコンテナ
	std::vector<std::unique_ptr<Canota>> canotas_;
	//キーキャノ太のコンテナ
	std::vector<std::unique_ptr<KeyCanota>> keyCanotas_;
	//ジェットのコンテナ
	std::vector<std::unique_ptr<Jet>> jets_;
	//ボスのインスタンス
	std::vector<std::unique_ptr<Boss>> bosses_;

};

