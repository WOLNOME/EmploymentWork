#pragma once
#include <Sprite.h>
#include <memory>

/// <summary>
/// 戦車内装のスプライトを管理するクラス
/// </summary>
class DecorativeUI {
public:
	//初期化
	void Initialize();

	//シェイクの適用
	void AttachShake(const Vector2& _shakeOffset);
	//点滅の適用
	void AttachBlinking(const Vector4& _color);

private:
	uint32_t textureHandle_ = 0u;
	std::unique_ptr<Sprite> sprite_ = nullptr;

};

