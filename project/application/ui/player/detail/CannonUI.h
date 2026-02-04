#pragma once
#include <Sprite.h>
#include <array>
#include <memory>
#include <JsonUtil.h>

class Player;

/// <summary>
/// 砲弾の装填状況UIを管理するクラス
/// </summary>
class CannonUI {
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
	/// 点滅の適用
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
	std::array<std::unique_ptr<Norm::Sprite>, 2> sprites_;
};

