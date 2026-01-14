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
class KeyCanota : public IBaseTankEnemy {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_isUseCannon">砲弾可能フラグ</param>
	KeyCanota(bool _isUseCannon) : IBaseTankEnemy(_isUseCannon) {}
	/// <summary>
	/// デストラクタ
	/// </summary>
	~KeyCanota() override {};
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
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="attribute">相手の属性</param>
	/// <param name="subjectPos">相手の座標</param>
	void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) override;
};

