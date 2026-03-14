#pragma once
#include "OBBColliderBase.h"
#include <Audio.h>
#include <memory>

class ItemHeal;

/// <summary>
/// 回復アイテムのコライダー
/// </summary>
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

	//SE
	std::unique_ptr<Norm::Audio> getSE_ = nullptr;	//取得
};

