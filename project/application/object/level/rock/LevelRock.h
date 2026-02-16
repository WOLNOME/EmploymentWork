#pragma once
#include "application/object/level/base/IBaseLevelObject.h"

/// <summary>
/// 岩オブジェクトの管理クラス
/// </summary>
class LevelRock : public IBaseLevelObject {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	LevelRock() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~LevelRock() override = default;
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

private:
	/// ============================== ///
	///		メンバ変数(private)
	/// ============================== ///


};

