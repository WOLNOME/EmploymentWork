#pragma once
#include "Collider.h"
#include "Object3d.h"

/// <summary>
/// キャラクター全般の基底クラス
/// </summary>
class BaseCharacter : public Collider {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BaseCharacter() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseCharacter() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();
	/// <summary>
	/// デバッグ用パラメータ調整
	/// </summary>
	virtual void DebugWithImGui();

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// ワールドトランスフォームの取得
	/// </summary>
	/// <returns>ワールドトランスフォーム</returns>
	const WorldTransform& GetWorldTransform() { return object3d_->worldTransform; }
	/// <summary>
	/// コライダー用のワールド座標を取得
	/// </summary>
	/// <returns>コライダー用のワールド座標</returns>
	Vector3 GetWorldPosition() override { return object3d_->worldTransform.worldTranslate; }
	/// <summary>
	/// コライダー用回転を取得
	/// </summary>
	/// <returns>コライダー用回転</returns>
	Vector3 GetRotate() override { return object3d_->worldTransform.rotate; }
	/// <summary>
	/// コライダー用スケールを取得
	/// </summary>
	/// <returns>コライダー用スケール</returns>
	Vector3 GetScale() override { return object3d_->worldTransform.scale; }
	/// <summary>
	/// コライダー用前フレーム座標を取得
	/// </summary>
	/// <returns>コライダー用前フレーム座標</returns>
	Vector3 GetPreWorldPosition() override { return prePosition_; }

	/// <summary>
	/// 死亡フラグの取得
	/// </summary>
	/// <returns>死亡フラグ</returns>
	bool GetIsDead() const { return isDead_; }
	/// <summary>
	/// 死亡タイマーの取得
	/// </summary>
	/// <returns>死亡タイマー</returns>
	float GetDeadTimer() const { return deadTimer_; }

	/// <summary>
	/// 速度の取得
	/// </summary>
	/// <returns>速度</returns>
	const Vector3& GetVelocity() { return velocity_; }
	/// <summary>
	/// 床の摩擦力の取得
	/// </summary>
	/// <returns>床の摩擦力</returns>
	float GetFloorFriction() const { return floorFriction_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// スケールのセット
	/// </summary>
	/// <param name="scale">スケール</param>
	void SetScale(const Vector3& scale) { object3d_->worldTransform.scale = scale; }
	/// <summary>
	/// 回転のセット
	/// </summary>
	/// <param name="rotate">回転</param>
	void SetRotate(const Vector3& rotate) { object3d_->worldTransform.rotate = rotate; }
	/// <summary>
	/// 平行移動のセット
	/// </summary>
	/// <param name="translate">平行移動</param>
	void SetTranslate(const Vector3& translate) { object3d_->worldTransform.translate = translate; }

	/// <summary>
	/// 死亡予約関数
	/// </summary>
	/// <param name="remainingSeconds">死亡までの時間</param>
	void SetDeadTimer(float remainingSeconds);

	/// <summary>
	/// 速度のセット
	/// </summary>
	/// <param name="_velocity">速度</param>
	void SetVelocity(const Vector3& _velocity) { velocity_ = _velocity; }

protected:
	/// ============================== ///
	///		メンバ変数(protected)
	/// ============================== ///

	//テクスチャハンドル
	int32_t textureHandle_ = EOF;
	//オブジェクト3D
	std::unique_ptr<Object3d> object3d_ = nullptr;
	//丸影
	std::unique_ptr<Object3d> circleShadow_ = nullptr;

	//前フレームの座標
	Vector3 prePosition_ = {};

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

private:
	/// ============================== ///
	///		メンバ変数(private)
	/// ============================== ///
	
	//死亡までの時間
	float deadTimer_ = 0.0f;
};

