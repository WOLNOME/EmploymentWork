#pragma once
#include <Input.h>
#include <Sprite.h>
#include <memory>
#include <JsonUtil.h>
#include <Handle.h>

class Player;

/// <summary>
/// 速度を示すレバーUIを管理するクラス
/// </summary>
class VelocityUI {
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
	/// プレイヤーの設定
	/// </summary>
	/// <param name="_player">プレイヤー</param>
	void SetPlayer(Player* _player) { player_ = _player; }


private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//プレイヤー
	Player* player_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json param_;
	//プレイヤーのパラメーター
	json playerParam_;


	//速度計
	uint32_t thSpeedMeter_ = 0;
	std::unique_ptr<Norm::Sprite> spriteSpeedMeter_;

	//針
	uint32_t thNeedle_ = 0;
	std::unique_ptr<Norm::Sprite> spriteNeedle_;

	//数値テキスト
	Norm::Handle thNumText_;
	std::unique_ptr<Norm::Sprite> spriteNumText_;

};

