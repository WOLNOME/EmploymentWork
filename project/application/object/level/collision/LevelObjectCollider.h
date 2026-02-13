#pragma once
#include "OBBColliderBase.h"

//前方宣言（アプリケーション）
class IBaseLevelObject;

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
	LevelObjectCollider(IBaseLevelObject* _holder);
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
	IBaseLevelObject* holder_ = nullptr;

};

