#pragma once
#include "SphereColliderBase.h"

class PlayerBullet;

/// <summary>
/// プレイヤー弾丸のコライダー
/// </summary>
class PlayerBulletCollider : public Norm::SphereColliderBase {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerBulletCollider(PlayerBullet* _holder);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerBulletCollider() override = default;

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
	PlayerBullet* holder_ = nullptr;

};

