#pragma once
#include "OBBColliderBase.h"

class Player;

/// <summary>
/// プレイヤーの当たり判定
/// </summary>
class PlayerCollider : public Norm::OBBColliderBase {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PlayerCollider(Player* _holder);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~PlayerCollider() override = default;

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
	Player* holder_ = nullptr;

};

