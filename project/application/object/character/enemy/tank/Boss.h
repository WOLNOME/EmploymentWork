#pragma once
#include "application/object/character/enemy/tank/base/IBaseTankEnemy.h"
#include "MyMath.h"
#include "Particle.h"
#include <Vector3.h>
#include <memory>

class Player;

/// <summary>
/// ボスエネミー単体の処理全般を管理するクラス
/// </summary>
class Boss : public IBaseTankEnemy {
public:
	//コンストラクタ
	Boss(bool _isUseCannon) : IBaseTankEnemy(_isUseCannon) {}
	//デストラクタ
	~Boss() override {};
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

