#pragma once
#include <Sprite.h>
#include <memory>
#include <JsonUtil.h>

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

	/// <summary>
	/// シェイクの適用
	/// </summary>
	/// <param name="_shakeOffset">シェイクオフセット</param>
	void AttachShake(const Norm::Vector2& _shakeOffset);
	/// <summary>
	/// 点滅の適用
	/// </summary>
	/// <param name="_color">色</param>
	void AttachBlinking(const Norm::Vector4& _color);

private:
	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json param_;

	uint32_t textureHandle_ = 0u;
	std::unique_ptr<Norm::Sprite> sprite_ = nullptr;
};

