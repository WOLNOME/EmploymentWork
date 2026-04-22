#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include <Audio.h>
#include "CombinedParticle.h"
#include "BulletTrail.h"
#include "JsonUtil.h"
#include <Vector3.h>
#include <memory>

class PlayerWeaponManager;

/// <summary>
/// プレイヤーの使う機関銃弾単体の処理全般を管理するクラス
/// </summary>
class PlayerBullet : public BaseCharacter {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerBullet() override {};
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui() override;

	/// <summary>
	/// スポーン
	/// </summary>
	/// <param name="_initPos">初期位置</param>
	/// <param name="_initDirection">初期方向</param>
	void Spawn(const Norm::Vector3& _initPos, const Norm::Vector3& _initDirection);

	/// <summary>
	/// 死亡処理を行う
	/// </summary>
	void DeadProcess();

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// プレイヤー武器管理クラスのインスタンスを取得する
	/// </summary>
	/// <returns>プレイヤー武器管理クラスのインスタンス</returns>
	PlayerWeaponManager* GetPlayerWeaponManager() { return playerWeaponManager_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// プレイヤー武器管理クラスのインスタンスをセットする
	/// </summary>
	/// <param name="_playerWeaponManager">プレイヤー武器管理クラスのインスタンス</param>
	void SetPlayerWeaponManager(PlayerWeaponManager* _playerWeaponManager) { playerWeaponManager_ = _playerWeaponManager; }

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// 移動処理を行う
	/// </summary>
	void Move();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	PlayerWeaponManager* playerWeaponManager_ = nullptr;	//プレイヤー武器管理クラスのインスタンス

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//SE
	std::unique_ptr<Norm::Audio> shotSE_ = nullptr;		//発射
	//パラメータ
	json param_;

	//トレールエフェクト
	std::unique_ptr<Norm::BulletTrail> trail_ = nullptr;
	//衝突エフェクト
	std::unique_ptr<Norm::CombinedParticle> hitEffect_ = nullptr;

	float lifeTimer_ = 0.0f;		//寿命タイマー

};

