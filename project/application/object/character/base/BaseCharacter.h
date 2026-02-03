#pragma once
#include "Collider.h"
#include "Object3d.h"

/// <summary>
/// キャラクター全般の基底クラス
/// </summary>
class BaseCharacter : public Norm::Collider {
public:
	/// ============================== ///
	///		列挙体
	/// ============================== ///

	/// <summary>
	/// 状態
	/// </summary>
	enum class State {
		kIdle,		//待機中（スポーン待ち）
		kActive,	//稼働中
		kAsphyxia,	//仮死中（演出等）
	};

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
	const Norm::WorldTransform& GetWorldTransform() { return object3d_->worldTransform; }
	/// <summary>
	/// コライダー用のワールド座標を取得
	/// </summary>
	/// <returns>コライダー用のワールド座標</returns>
	Norm::Vector3 GetWorldPosition() override { return object3d_->worldTransform.GetWorldTranslate(); }
	/// <summary>
	/// コライダー用回転を取得
	/// </summary>
	/// <returns>コライダー用回転</returns>
	Norm::Vector3 GetRotate() override { return object3d_->worldTransform.GetRotate(); }
	/// <summary>
	/// コライダー用スケールを取得
	/// </summary>
	/// <returns>コライダー用スケール</returns>
	Norm::Vector3 GetScale() override { return object3d_->worldTransform.GetScale(); }
	/// <summary>
	/// コライダー用前フレーム座標を取得
	/// </summary>
	/// <returns>コライダー用前フレーム座標</returns>
	Norm::Vector3 GetPreWorldPosition() override { return prePosition_; }

	/// <summary>
	/// 状態の取得
	/// </summary>
	/// <returns>状態</returns>
	State GetState() const { return state_; }

	/// <summary>
	/// 速度の取得
	/// </summary>
	/// <returns>速度</returns>
	const Norm::Vector3& GetVelocity() { return velocity_; }
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
	void SetScale(const Norm::Vector3& scale) { object3d_->worldTransform.SetScale(scale); }
	/// <summary>
	/// 回転のセット
	/// </summary>
	/// <param name="rotate">回転</param>
	void SetRotate(const Norm::Vector3& rotate) { object3d_->worldTransform.SetRotate(rotate); }
	/// <summary>
	/// 平行移動のセット
	/// </summary>
	/// <param name="translate">平行移動</param>
	void SetTranslate(const Norm::Vector3& translate) { object3d_->worldTransform.SetTranslate(translate); }

	/// <summary>
	/// 状態のセット
	/// </summary>
	/// <param name="_state">状態</param>
	void SetState(const State& _state);

	/// <summary>
	/// 速度のセット
	/// </summary>
	/// <param name="_velocity">速度</param>
	void SetVelocity(const Norm::Vector3& _velocity) { velocity_ = _velocity; }

protected:
	/// ============================== ///
	///		メンバ変数(protected)
	/// ============================== ///

	//テクスチャハンドル
	int32_t textureHandle_ = EOF;
	//オブジェクト3D
	std::unique_ptr<Norm::Object3d> object3d_ = nullptr;
	//丸影
	std::unique_ptr<Norm::Object3d> circleShadow_ = nullptr;

	//状態
	State state_ = State::kIdle;

	//前フレームの座標
	Norm::Vector3 prePosition_ = {};

	//速度
	Norm::Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };

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

};

