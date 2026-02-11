#pragma once
#include "SphereColliderBase.h"

class PlayerSpecial;

/// <summary>
/// プレイヤー必殺弾のコライダー
/// </summary>
class PlayerSpecialCollider : public Norm::SphereColliderBase {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerSpecialCollider(PlayerSpecial* _holder);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerSpecialCollider() override = default;

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
	PlayerSpecial* holder_ = nullptr;

};

