#pragma once
#include <Sprite.h>
#include <memory>
#include <JsonUtil.h>

//前方宣言（エンジン）
namespace Norm {
	class GameCamera;
}

/// <summary>
/// カメラの向いている方向を示すUIを管理するクラス
/// </summary>
class DirectionUI {
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
	/// ゲームカメラの設定
	/// </summary>
	/// <param name="_camera">ゲームカメラのポインタ</param>
	void SetGameCamera(Norm::GameCamera* _camera) { camera_ = _camera; }

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	Norm::GameCamera* camera_ = nullptr;

private:
	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json param_;

	//矢印
	uint32_t thArrow_;
	std::unique_ptr<Norm::Sprite> spriteArrow_;

	//目盛り
	uint32_t thScale_;
	std::unique_ptr<Norm::Sprite> spriteScale_;


};

