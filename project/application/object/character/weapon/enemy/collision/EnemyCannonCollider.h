#pragma once
#include "SphereColliderBase.h"

class EnemyCannon;

/// <summary>
/// 敵砲弾のコライダー
/// </summary>
class EnemyCannonCollider : public Norm::SphereColliderBase {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyCannonCollider(EnemyCannon* _holder);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyCannonCollider() override = default;

	/// <summary>
	/// デバッグ
	/// </summary>
	void Debug() override;

	/// <summary>
	/// 衝突時のコールバック
	/// </summary>
	/// <param name="_other">衝突相手のコライダー</param>
	void OnCollision(Norm::ICollider* _other, Norm::CollisionAttribute _attribute) override;

private:
	//所有者のポインタ
	EnemyCannon* holder_ = nullptr;

};

