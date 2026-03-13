#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include <Audio.h>
#include "CombinedParticle.h"
#include "BulletTrail.h"
#include "JsonUtil.h"
#include <Vector3.h>
#include <memory>

//前方宣言（アプリケーション）
class PlayerUI;
class CameraManager;

/// <summary>
/// 敵の使う機関銃弾単体の処理全般を管理するクラス
/// </summary>
class EnemyBullet : public BaseCharacter {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBullet() override {};
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
	void Spawn(const Norm::Vector3& _initPos, const Norm::Vector3& _targetPos);

	/// <summary>
	/// 死亡処理を行う
	/// </summary>
	void DeadProcess();

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// プレイヤーUIを取得する
	/// </summary>
	/// <returns>プレイヤーUI</returns>
	PlayerUI* GetPlayerUI() { return playerUI_; }
	/// <summary>
	/// 生成された場所を取得する
	/// </summary>
	/// <returns>生成された場所</returns>
	const Norm::Vector3& GetGeneratedPosition() { return generatedPosition_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// プレイヤーUIを設定する
	/// </summary>
	/// <param name="_playerUI">設定するプレイヤーUI</param>
	void SetPlayerUI(PlayerUI* _playerUI) { playerUI_ = _playerUI; }
	/// <summary>
	/// カメラマネージャーのセット
	/// </summary>
	/// <param name="_cameraManager">カメラマネージャー</param>
	void SetCameraManager(CameraManager* _cameraManager) { cameraManager_ = _cameraManager; }

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

	//プレイヤーUI
	PlayerUI* playerUI_ = nullptr;
	//カメラ
	CameraManager* cameraManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//SE
	std::unique_ptr<Norm::Audio> shotSE_ = nullptr;		//発射

	//パラメータ
	json param_;
	json audioParam_;

	//トレールエフェクト
	std::unique_ptr<Norm::BulletTrail> trail_ = nullptr;
	//衝突エフェクト
	std::unique_ptr<Norm::CombinedParticle> hitEffect_ = nullptr;

	Norm::Vector3 generatedPosition_{};	//生成された座標
	float lifeTimer_;		//寿命タイマー

};

