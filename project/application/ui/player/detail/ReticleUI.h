#pragma once
#include <Sprite.h>
#include <memory>

/// <summary>
/// レティクルUIを管理するクラス
/// </summary>
class ReticleUI {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

private:
	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	uint32_t textureHandle_ = 0u;
	std::unique_ptr<Norm::Sprite> sprite_ = nullptr;
};

