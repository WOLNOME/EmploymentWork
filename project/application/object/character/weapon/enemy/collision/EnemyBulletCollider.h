#pragma once
#include "SphereColliderBase.h"

class EnemyBullet;

/// <summary>
/// 敵弾丸のコライダー
/// </summary>
class EnemyBulletCollider : public Norm::SphereColliderBase {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	EnemyBulletCollider(EnemyBullet* _holder);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBulletCollider() override = default;

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
	EnemyBullet* holder_ = nullptr;

};

