#pragma once
#include "application/object/character/enemy/base/IBaseEnemy.h"
#include "MyMath.h"
#include "Particle.h"
#include <Vector3.h>
#include <memory>

class Player;
class Canota : public IBaseEnemy {
public:
	//デストラクタ
	~Canota() override {};
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
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

public://getter
	bool GetIsCannonFire() const { return isCannonFire_; }

private://非公開関数
	//攻撃
	void Attack();

private://メンバ変数
	//砲弾攻撃
	float cannonCoolTime_;
	float cannonCoolTimer_;
	bool isCannonFire_ = false;

};

