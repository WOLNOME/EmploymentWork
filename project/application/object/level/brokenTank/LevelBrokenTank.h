#pragma once
#include "application/object/level/base/IBaseLevelObject.h"
#include <CombinedParticle.h>
#include <memory>
#include <vector>

/// <summary>
/// 破壊された戦車の管理クラス
/// </summary>
class LevelBrokenTank : public IBaseLevelObject {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	LevelBrokenTank() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~LevelBrokenTank() override = default;
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_name">名前</param>
	void Initialize(const std::string& _name) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// ImGui操作
	/// </summary>
	void DebugWithImGui() override;

	/// <summary>
	/// パーティクルの初期化
	/// </summary>
	void InitParticle();

private:
	/// ============================== ///
	///		メンバ変数(private)
	/// ============================== ///

	//パーティクル
	std::vector<std::unique_ptr<Norm::CombinedParticle>> particles_;


};

