#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "BaseCamera.h"
#include "SceneLight.h"
#include "Object3d.h"
#include "CombinedParticle.h"
#include "BulletTrail.h"
#include "JsonUtil.h"
#include <Vector3.h>
#include <memory>

/// <summary>
/// プレイヤーの使う砲弾単体の処理全般を管理するクラス
/// </summary>
class PlayerCannon : public BaseCharacter {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerCannon() override {};
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

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// 爆発パーティクルを取得する
	/// </summary>
	/// <returns>爆発パーティクル</returns>
	Norm::CombinedParticle* GetExplosionParticle() { return explosionParticle_.get(); }

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

	//爆発パーティクル
	std::unique_ptr<Norm::CombinedParticle> explosionParticle_ = nullptr;
	//トレールエフェクト
	std::unique_ptr<Norm::BulletTrail> trail_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメータ
	json param_;

};

