#pragma once
#include "OBBColliderBase.h"
#include <Audio.h>
#include <memory>

class ItemCharge;

/// <summary>
/// 必殺弾チャージアイテムのコライダー
/// </summary>
class ItemChargeCollider : public Norm::OBBColliderBase {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ItemChargeCollider(ItemCharge* _holder);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ItemChargeCollider() override = default;

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
	ItemCharge* holder_ = nullptr;

	//SE
	std::unique_ptr<Norm::Audio> getSE_ = nullptr;	//取得
};

