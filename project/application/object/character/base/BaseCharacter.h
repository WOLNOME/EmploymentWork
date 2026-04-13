#pragma once
#include "Object3d.h"
#include "WorldTransform.h"

//前方宣言（エンジン）
namespace Norm {
	class ICollider;
};

/// <summary>
/// キャラクター全般の基底クラス
/// </summary>
class BaseCharacter{
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
		kStillness,	//静止中（移動・攻撃不能状態）
		kAsphyxia,	//仮死中（演出等）
	};

public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BaseCharacter();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~BaseCharacter();
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
	const Norm::WorldTransform& GetWorldTransform() { return worldTransform_; }
	
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
	void SetScale(const Norm::Vector3& scale) { worldTransform_.SetScale(scale); }
	/// <summary>
	/// 回転のセット
	/// </summary>
	/// <param name="rotate">回転</param>
	void SetRotate(const Norm::Vector3& rotate) { worldTransform_.SetRotate(rotate); }
	/// <summary>
	/// 平行移動のセット
	/// </summary>
	/// <param name="translate">平行移動</param>
	void SetTranslate(const Norm::Vector3& translate) { worldTransform_.SetTranslate(translate); }

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
	//ワールドトランスフォーム
	Norm::WorldTransform worldTransform_;

	//丸影
	std::unique_ptr<Norm::Object3d> circleShadow_ = nullptr;
	//丸影のワールドトランスフォーム
	Norm::WorldTransform csWorldTransform_;

	//当たり判定
	std::unique_ptr<Norm::ICollider> collider_ = nullptr;

	//状態
	State state_ = State::kIdle;

	//速度
	Norm::Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };

	//重力値
	float gravity_ = 33.0f;
	//空気抵抗値
	float airResistance_ = 1.0f;
	//床の摩擦値
	const float floorFriction_ = 80.0f;

private:
	/// ============================== ///
	///		メンバ変数(private)
	/// ============================== ///

};

