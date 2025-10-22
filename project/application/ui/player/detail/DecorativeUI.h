#pragma once
#include <Sprite.h>
#include <memory>

/// <summary>
/// 戦車内装のスプライトを管理するクラス
/// </summary>
class DecorativeUI {
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
	void AttachShake(const Vector2& _shakeOffset);
	/// <summary>
	/// 点滅の適用
	/// </summary>
	/// <param name="_color">色</param>
	void AttachBlinking(const Vector4& _color);

private:
	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	uint32_t textureHandle_ = 0u;
	std::unique_ptr<Sprite> sprite_ = nullptr;

};

