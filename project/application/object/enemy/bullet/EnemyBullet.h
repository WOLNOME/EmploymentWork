#pragma once
#include "application/object/baseCharacter/BaseCharacter.h"
#include "BaseCamera.h"
#include "SceneLight.h"
#include "Object3d.h"
#include "Particle.h"
#include <memory>

class EnemyBullet : public BaseCharacter {
public:
	//デストラクタ
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
	void SetInitParam(const Vector3& _initPos, const Vector3& _targetPos);
private:
	//パーティクル
	std::unique_ptr<Particle> particle_ = nullptr;

private:
	//移動処理
	void Move();
private:
	const float maxHeight_ = 3.5f;		//最大高度
	const float hitTime_ = 0.9f;		//着弾秒数



};

