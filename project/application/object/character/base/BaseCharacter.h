#pragma once
#include "Collider.h"
#include "Object3d.h"

//キャラクター基底クラス
class BaseCharacter : public Collider {
public:
	//コンストラクタ
	BaseCharacter() = default;
	//デストラクタ
	virtual ~BaseCharacter() = default;
	//更新
	virtual void Update();

	//デバッグ用パラメーター調整
	virtual void DebugWithImGui();

public://getter
	//ワールドトランスフォームの取得
	const WorldTransform& GetWorldTransform() { return object3d_->worldTransform; }
	//コライダー用のワールド座標を取得
	Vector3 GetWorldPosition() override { return object3d_->worldTransform.worldTranslate; }
	//コライダー用回転を取得
	Vector3 GetRotate() override { return object3d_->worldTransform.rotate; }
	//コライダー用スケールを取得
	Vector3 GetScale() override { return object3d_->worldTransform.scale; }
	//コライダー用前フレーム座標を取得
	Vector3 GetPreWorldPosition() override { return prePosition_; }

	//死亡フラグの取得
	bool GetIsDead() const { return isDead_; }
	//死亡タイマーの取得
	float GetDeadTimer() const { return deadTimer_; }

	//速度の取得
	const Vector3& GetVelocity() { return velocity_; }
	//床の摩擦力の取得
	float GetFloorFriction() const { return floorFriction_; }

public://setter
	//スケールのセット
	void SetScale(const Vector3& scale) { object3d_->worldTransform.scale = scale; }
	//回転のセット
	void SetRotate(const Vector3& rotate) { object3d_->worldTransform.rotate = rotate; }
	//平行移動のセット
	void SetTranslate(const Vector3& translate) { object3d_->worldTransform.translate = translate; }

	//死亡予約関数
	void SetDeadTimer(float remainingSeconds);

	//速度のセット
	void SetVelocity(const Vector3& _velocity) { velocity_ = _velocity; }

protected://オブジェクト
	//テクスチャハンドル
	int32_t textureHandle_ = EOF;
	//オブジェクト3D
	std::unique_ptr<Object3d> object3d_ = nullptr;
	//前フレームの座標
	Vector3 prePosition_ = {};

protected://キャラ共有のメンバ変数
	//速度
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };
	//死亡フラグ
	bool isDead_ = false;

	//重力値
	float gravity_ = 33.0f;
	//空気抵抗値
	float airResistance_ = 1.0f;
	//床の摩擦値
	float floorFriction_ = 40.0f;

private://死亡予約用変数
	float deadTimer_ = 0.0f;	//死亡までの時間
};

