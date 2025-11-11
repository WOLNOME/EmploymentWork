#pragma once
#include <Input.h>
#include <Sprite.h>
#include <array>
#include <memory>
#include <JsonUtil.h>

/// <summary>
/// 移動量(速度)を示すレバーUIを管理するクラス
/// </summary>
class MoveLever {
private:
	/// ============================== ///
	///		列挙体
	/// ============================== ///

	enum class Type {
		ControlScale,	//制御目盛り
		Lever,			//レバー

		kMaxTypeNum,	//タイプの数
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
	///		インスタンス
	/// ============================== ///

	//入力
	Input* input_ = Input::GetInstance();

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	std::array<uint32_t, (int)Type::kMaxTypeNum> textureHandles_;
	std::array<std::unique_ptr<Sprite>, (int)Type::kMaxTypeNum> sprites_;

	//ゲージ(0~500)
	const int maxGauge_ = 250;
	int gauge_ = 125;
};

