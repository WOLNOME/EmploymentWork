#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "BaseCamera.h"
#include "SceneLight.h"
#include "Object3d.h"
#include "Particle.h"
#include "JsonUtil.h"
#include <Vector3.h>
#include <memory>


class EnemyCannon : public BaseCharacter {
public:
	//デストラクタ
	~EnemyCannon() override {};
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
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
	void SetInitParam(const Vector3& _initPos, const Vector3& _targetPos);
private:
	//パーティクル
	std::unique_ptr<Particle> particle_ = nullptr;

private:
	//移動処理
	void Move();
private:
	//パラメータ
	json param_;


};

