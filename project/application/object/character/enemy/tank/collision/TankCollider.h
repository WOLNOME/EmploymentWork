#pragma once
#include "OBBColliderBase.h"

class IBaseTankEnemy;

class TankCollider : public Norm::OBBColliderBase {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TankCollider(IBaseTankEnemy* _holder);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~TankCollider() override = default;

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
	IBaseTankEnemy* holder_ = nullptr;

};

