#pragma once
#include "application/object/character/enemy/tank/base/IBaseTankEnemy.h"
#include "MyMath.h"
#include <Vector3.h>
#include <memory>

class Canota : public IBaseTankEnemy {
public:
	//コンストラクタ
	Canota(bool _isUseCannon) : IBaseTankEnemy(_isUseCannon) {}
	//デストラクタ
	~Canota() override {};
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui() override;

private:
	//当たり判定処理
	void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) override;

};

