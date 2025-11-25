#pragma once
#include <array>
#include <Sprite.h>
#include <memory>

/// <summary>
/// 操作方法のUIを管理するクラス
/// </summary>
class OperationUI {
private:
	/// ============================== ///
	///		列挙体
	/// ============================== ///

	enum class Type {
		kCannon,			//砲弾攻撃
		kBullet,			//機関銃攻撃
		kPerspective,		//視点移動
		kMove,				//移動

		kMaxTypeNum,		//タイプの数
	};

public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// シェイクの適用
	/// </summary>
	/// <param name="_shakeOffset">シェイクオフセット</param>
	void AttachShake(const Vector2& _shakeOffset);
	/// <summary>
	/// 点滅エフェクトの適用
	/// </summary>
	/// <param name="_color">色</param>
	void AttachBlinking(const Vector4& _color);

private:
	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	std::array<uint32_t, (int)Type::kMaxTypeNum> textureHandles_;
	std::array<std::unique_ptr<Sprite>, (int)Type::kMaxTypeNum> sprites_;
};

