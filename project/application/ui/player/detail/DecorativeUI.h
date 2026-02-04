#pragma once
#include <Sprite.h>
#include <memory>

//アプリケーション
#include <application/object/character/player/player.h>

/// <summary>
/// 戦車内装のスプライトを管理するクラス
/// </summary>
class DecorativeUI {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DecorativeUI();
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
	/// <param name="_player">プレイヤーインスタンス</param>
	void SetPlayer(Player* _player) { player_ = _player; }

private:
	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//プレイヤー
	Player* player_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//スプライト
	uint32_t textureHandleN_ = 0u;
	uint32_t textureHandleD_ = 0u;
	std::unique_ptr<Norm::Sprite> sprite_ = nullptr;

	//通常状態→ダメージ状態UVスクロール
	uint32_t textureHandleND_ = 0u;
	std::unique_ptr<Norm::Sprite> uvScroll_ = nullptr;
};

