#pragma once
#include "OBBColliderBase.h"
#include <Audio.h>
#include <memory>

class ItemKey;

class ItemKeyCollider : public Norm::OBBColliderBase {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ItemKeyCollider(ItemKey* _holder);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ItemKeyCollider() override = default;

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
	ItemKey* holder_ = nullptr;

	//SE
	std::unique_ptr<Norm::Audio> getSE_ = nullptr;	//取得
};

