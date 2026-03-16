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
/// 敵の使う砲弾の処理全般を管理するクラス
/// </summary>
class EnemyCannon : public BaseCharacter {
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
	~EnemyCannon() override {};
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
	/// <param name="_size">サイズ</param>
	void Spawn(const Norm::Vector3& _initPos, const Norm::Vector3& _targetPos, float _size = 1.0f);

	/// <summary>
	/// 死亡処理
	/// </summary>
	/// <param name="_deadType">死亡タイプ</param>
	void DeadProcess(DeadType _deadType);

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
	std::unique_ptr<Norm::Audio> deadSE_ = nullptr;		//死亡

	//パラメータ-
	json param_;
	json audioParam_;

	//爆発パーティクル
	std::unique_ptr<Norm::CombinedParticle> explosionParticle_ = nullptr;
	//地面衝突パーティクル
	std::unique_ptr<Norm::CombinedParticle> groundParticle_ = nullptr;
	//発射時パーティクル
	std::unique_ptr<Norm::CombinedParticle> fireParticle_ = nullptr;

	//トレールエフェクト
	std::unique_ptr<Norm::BulletTrail> trail_ = nullptr;


	//生成された座標
	Norm::Vector3 generatedPosition_ = {};

};

