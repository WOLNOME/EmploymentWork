#pragma once
#include <Sprite.h>
#include <memory>
#include <JsonUtil.h>
#include <Handle.h>

class Player;

/// <summary>
/// プレイヤーのHPUIを管理するクラス
/// </summary>
class PlayerHPUI {
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

private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	Player* player_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json param_;
	//プレイヤーのパラメーター
	json playerParam_;

	//HPバー
	uint32_t thBar_;
	std::unique_ptr<Norm::Sprite> spriteBar_;

	//HPテキスト
	Norm::Handle thHPText_;
	std::unique_ptr<Norm::Sprite> spriteHPText_;
};

