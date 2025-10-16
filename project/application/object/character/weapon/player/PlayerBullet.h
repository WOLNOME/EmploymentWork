#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "BaseCamera.h"
#include "SceneLight.h"
#include "Object3d.h"
#include "Particle.h"
#include "BulletTrail.h"
#include "JsonUtil.h"
#include <Vector3.h>
#include <memory>

/// <summary>
/// プレイヤーの使う機関銃弾単体の処理全般を管理するクラス
/// </summary>
class PlayerBullet : public BaseCharacter {
public:
	//デストラクタ
	~PlayerBullet() override {};
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	//ImGuiデバッグ
	void DebugWithImGui() override;

	//当たり判定処理
	void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) override;

public://setter
	/// 最初にセットするパラメーター
	void SetInitParam(const Vector3& _initPos, const Vector3& _initDirection);

private:
	//トレールエフェクト
	std::unique_ptr<BulletTrail> trail_ = nullptr;

private:
	//移動処理
	void Move();

	//死亡処理
	void DeadProcess();
private:
	//パラメータ
	json param_;

	float lifeTime_;		//寿命
	float lifeTimer_;		//寿命タイマー

};

