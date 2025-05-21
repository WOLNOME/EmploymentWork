#pragma once
#include "application/object/baseCharacter/BaseCharacter.h"
#include "BaseCamera.h"
#include "SceneLight.h"
#include "Object3d.h"
#include "Particle.h"
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
	/// <summary>
	/// 最初にセットするパラメーター
	/// </summary>
	/// <param name="_initPos">初期位置</param>
	/// <param name="_initVelocity">初速度</param>
	void SetInitParam(const Vector3& _initPos, const Vector3& _initVelocity) {
		object3d_->worldTransform.translate = _initPos;
		velocity_ = _initVelocity;
	}
private:
	//パーティクル
	std::unique_ptr<Particle> particle_ = nullptr;

private:
	//移動処理
	void Move();
private:
	//変数
	const float maxSpeed_ = 300.0f;					//最大移動スピード
};

