#pragma once
#include <Sprite.h>
#include <array>
#include <memory>
#include <JsonUtil.h>

class Player;

/// <summary>
/// 機関銃弾の装填状況UIを管理するクラス
/// </summary>
class BulletUI {
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

	//プレイヤーのパラメーター
	json param_;

	std::array<uint32_t, 2> textureHandles_;
	std::array<std::unique_ptr<Sprite>, 2> sprites_;
};

