#pragma once
#include "OBBColliderBase.h"

class Boss;

/// <summary>
/// ボスの当たり判定
/// </summary>
class BossCollider : public Norm::OBBColliderBase {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BossCollider(Boss* _holder);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~BossCollider() override = default;

	/// <summary>
	/// 衝突時のコールバック
	/// </summary>
	/// <param name="_other"></param>
	void OnCollision(Norm::ICollider* _other, Norm::CollisionAttribute _attribute) override;

private:
	//所有者のポインタ
	Boss* holder_ = nullptr;

};

