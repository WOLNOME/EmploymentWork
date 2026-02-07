#pragma once
#include "OBBColliderBase.h"

//前方宣言（エンジン）
namespace Norm {
	class LevelObject;
};

/// <summary>
/// レベルオブジェクトのコライダー
/// </summary>
class LevelObjectCollider : public Norm::OBBColliderBase {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	LevelObjectCollider(Norm::LevelObject* _holder);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~LevelObjectCollider() override = default;

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
	Norm::LevelObject* holder_ = nullptr;

};

