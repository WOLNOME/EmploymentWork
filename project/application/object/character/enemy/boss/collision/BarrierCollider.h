#pragma once
#include "SphereColliderBase.h"

class Barrier;

/// <summary>
/// バリアの当たり判定
/// </summary>
class BarrierCollider : public Norm::SphereColliderBase {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BarrierCollider(Barrier* _holder);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~BarrierCollider() override = default;

	/// <summary>
	/// デバッグ
	/// </summary>
	void Debug() override;

	/// <summary>
	/// 衝突時のコールバック
	/// </summary>
	/// <param name="_other"></param>
	void OnCollision(Norm::ICollider* _other, Norm::CollisionAttribute _attribute) override;

private:
	//所有者のポインタ
	Barrier* holder_ = nullptr;

};

