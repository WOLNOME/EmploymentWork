#pragma once
#include "application/object/character/base/BaseCharacter.h"

class Boss : public BaseCharacter {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Boss() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Boss();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// デバッグ用パラメータ調整
	/// </summary>
	void DebugWithImGui() override;

};

