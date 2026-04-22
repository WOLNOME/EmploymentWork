#pragma once
#include <JsonUtil.h>
#include <Sprite.h>
#include <memory>

/// <summary>
/// 操作方法のUIを管理するクラス
/// </summary>
class OperationUI {
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
	void AttachShake(const Norm::Vector2& _shakeOffset);
	/// <summary>
	/// 点滅エフェクトの適用
	/// </summary>
	/// <param name="_color">色</param>
	void AttachBlinking(const Norm::Vector4& _color);

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// 表示するかを設定
	/// </summary>
	/// <param name="_isDisplay">表示するか</param>
	void SetIsDisplay(bool _isDisplay);

private:
	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json param_;

	//中心座標
	Norm::Vector2 centerPos_;

	//スプライト
	uint32_t textureHandle_;
	std::unique_ptr<Norm::Sprite> sprite_;
};

