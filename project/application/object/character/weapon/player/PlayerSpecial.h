#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include <Audio.h>
#include "BaseCamera.h"
#include "SceneLight.h"
#include "Object3d.h"
#include "CombinedParticle.h"
#include "BulletTrail.h"
#include "JsonUtil.h"
#include <Vector3.h>
#include <memory>

//前方宣言
class CameraManager;
class PlayerWeaponManager;

/// <summary>
/// プレイヤーの使う必殺弾単体の処理全般を管理するクラス
/// </summary>
class PlayerSpecial : public BaseCharacter {
public:
	/// ============================== ///
	///		列挙型
	/// ============================== ///

	//死亡タイプ
	enum class DeadType {
		Collide,	//オブジェクトにぶつかった場合
		Ground,		//地面にぶつかった場合
	};

	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerSpecial() override {};
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
	/// <param name="_targetPos">目標位置</param>
	void Spawn(const Norm::Vector3& _initPos, const Norm::Vector3& _initDirection);

	/// <summary>
	/// 死亡処理
	/// </summary>
	/// <param name="_deadType">死亡タイプ</param>
	void DeadProcess(DeadType _deadType);

	/// ============================== ///
	///		getter
	/// ============================== ///
	
	/// <summary>
	/// プレイヤー武器マネージャーの取得
	/// </summary>
	/// <returns>プレイヤー武器マネージャー</returns>
	PlayerWeaponManager* GetPlayerWeaponManager() { return playerWeaponManager_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// カメラマネージャーのセット
	/// </summary>
	/// <param name="_cameraManager">カメラマネージャー</param>
	void SetCameraManager(CameraManager* _cameraManager) { cameraManager_ = _cameraManager; }
	/// <summary>
	/// プレイヤー武器マネージャーのセット
	/// </summary>
	/// <param name="_playerWeaponManager">プレイヤー武器マネージャー</param>
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

	//カメラ
	CameraManager* cameraManager_ = nullptr;
	//プレイヤー武器マネージャー
	PlayerWeaponManager* playerWeaponManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//SE
	std::unique_ptr<Norm::Audio> shotSE_ = nullptr;		//発射
	std::unique_ptr<Norm::Audio> deadSE_ = nullptr;		//死亡

	//パラメータ
	json param_;
	json audioParam_;

	//爆発パーティクル
	std::unique_ptr<Norm::CombinedParticle> explosionParticle_ = nullptr;
	//地面衝突パーティクル
	std::unique_ptr<Norm::CombinedParticle> groundParticle_ = nullptr;
	//発射時パーティクル
	std::unique_ptr<Norm::CombinedParticle> fireParticle_ = nullptr;
	//粒パーティクル
	std::unique_ptr<Norm::CombinedParticle> grainParticle_ = nullptr;

	//トレールエフェクト
	std::unique_ptr<Norm::BulletTrail> trail_ = nullptr;

};

