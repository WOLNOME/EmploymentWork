#pragma once
#include "OBBColliderBase.h"

class ItemHeal;

class ItemHealCollider : public Norm::OBBColliderBase {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ItemHealCollider(ItemHeal* _holder);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ItemHealCollider() override = default;

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
	ItemHeal* holder_ = nullptr;
};

