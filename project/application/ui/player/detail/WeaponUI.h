#pragma once
#include <Sprite.h>
#include <memory>
#include <JsonUtil.h>

class Player;

/// <summary>
/// 武器の状況UIを管理するクラス
/// </summary>
class WeaponUI {
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
	/// <param name="_player"></param>
	void SetPlayer(Player* _player) { player_ = _player; }

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	Player* player_ = nullptr;

private:
	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json param_;
	//プレイヤーのパラメーター
	json playerParam_;

	//砲弾
	uint32_t thCannon_;
	std::unique_ptr<Norm::Sprite> spriteCannon_;

	//銃弾
	uint32_t thBullet_;
	std::unique_ptr<Norm::Sprite> spriteBullet_;

	//スペシャル
	uint32_t thSpecial_;
	std::unique_ptr<Norm::Sprite> spriteSpecial_;
};

