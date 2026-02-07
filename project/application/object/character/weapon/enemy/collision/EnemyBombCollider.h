#pragma once
#include "SphereColliderBase.h"

class EnemyBomb;

/// <summary>
/// 敵爆弾のコライダー
/// </summary>
class EnemyBombCollider : public Norm::SphereColliderBase {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyBombCollider(EnemyBomb* _holder);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBombCollider() override = default;

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
	EnemyBomb* holder_ = nullptr;

};

