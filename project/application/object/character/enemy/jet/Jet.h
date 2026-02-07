#pragma once
#include "application/object/character/enemy/jet/base/IBaseJetEnemy.h"
#include "MyMath.h"
#include "Particle.h"
#include <Vector3.h>
#include <memory>

/// <summary>
/// ジェットエネミー単体の処理全般を管理するクラス
/// </summary>
class Jet : public IBaseJetEnemy {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Jet() override {};
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

	/// <summary>
	/// スポーン
	/// </summary>
	/// <param name="_initPos">初期位置</param>
	/// <param name="_initRotate">初期回転</param>
	void Spawn(const Norm::Vector3& _initPos, const Norm::Vector3& _initRotate) override;

private:
	
};

