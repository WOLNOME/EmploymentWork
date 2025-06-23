#pragma once
#include "application/object/baseCharacter/BaseCharacter.h"
#include "BaseCamera.h"
#include "SceneLight.h"
#include "Object3d.h"
#include "Particle.h"
#include "JsonUtil.h"
#include <memory>

class PlayerBullet : public BaseCharacter {
public:
	//デストラクタ
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
	/// 描画
	/// </summary>
	void Draw() override;
	/// <summary>
	/// ライン描画
	/// </summary>
	void DrawLine() override;

	//ImGuiデバッグ
	void DebugWithImGui() override;

	//当たり判定処理
	void OnCollision(CollisionAttribute attribute) override;

public://setter
	/// 最初にセットするパラメーター
	void SetInitParam(const Vector3& _initPos, const Vector3& _initDirection);

private:
	//パーティクル
	std::unique_ptr<Particle> trail_ = nullptr;		//トレイルパーティクル

private:
	//移動処理
	void Move();
	//パーティクル
	void UpdateParticle();

	//死亡処理
	void DeadProcess();
private:
	//パラメータ
	json param_;

	float lifeTime_;		//寿命
	float lifeTimer_;		//寿命タイマー

};

