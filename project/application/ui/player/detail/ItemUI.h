#pragma once
#include <Sprite.h>
#include <array>
#include <memory>
#include <JsonUtil.h>

class Player;

/// <summary>
/// アイテム取得状況UIを管理するクラス
/// </summary>
class ItemUI {
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
	/// 点滅の適用
	/// </summary>
	/// <param name="_color">色</param>
	void AttachBlinking(const Vector4& _color);

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

	Player* player_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//プレイヤーのパラメーター
	json param_;

	std::array<uint32_t, 3> textureHandles_;
	std::array<std::unique_ptr<Sprite>, 3> sprites_;
	const int kTextureWidth_ = 680;
	const int kTextureHeight_ = 48;
	const int kSpriteStateNum_ = 4;	//0,1,2,3の4状態

};

