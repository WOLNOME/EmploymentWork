#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "BaseCamera.h"
#include "SceneLight.h"
#include "Object3d.h"
#include "Particle.h"
#include "JsonUtil.h"
#include <Vector3.h>
#include <memory>

class PlayerUI;
class EnemyBomb : public BaseCharacter {
public:
	//デストラクタ
	~EnemyBomb() override {};

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
	void SetInitParam(const Vector3& _initPos, const Vector3& _targetPos);

	//プレイヤーUIをセット
	void SetPlayerUI(PlayerUI* _playerUI) { playerUI_ = _playerUI; }

private:
	//プレイヤーUI
	PlayerUI* playerUI_ = nullptr;

	//パーティクル
	std::unique_ptr<Particle> particle_ = nullptr;
private:
	//移動処理
	void Move();
	//爆風
	void Blast();

private:
	//パラメータ
	json param_;

	//生成された座標
	Vector3 generatedPosition_ = {};

	//爆風
	bool isBlast_ = false;
	const float durationTime_ = 1.0f;
	float durationTimer_ = 0.0f;
};

