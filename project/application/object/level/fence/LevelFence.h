#pragma once
#include "application/object/level/base/IBaseLevelObject.h"

/// <summary>
/// 柵オブジェクトの管理クラス
/// </summary>
class LevelFence : public IBaseLevelObject {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	LevelFence() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~LevelFence() override = default;
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

