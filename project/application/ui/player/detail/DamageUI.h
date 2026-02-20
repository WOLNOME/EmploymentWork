#pragma once
#include <Sprite.h>
#include <memory>
#include <JsonUtil.h>

//アプリケーション
#include <application/object/character/player/player.h>

/// <summary>
/// ダメージ表現スプライトを管理するクラス
/// </summary>
class DamageUI {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DamageUI();
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

	//パラメーター
	json param_;

	//窓ガラスが割れている表現
	uint32_t thGlass_;
	std::unique_ptr<Norm::Sprite> spriteGrass_;

	//死亡時のガラス演出
	uint32_t thDeathDir_;
	std::unique_ptr<Norm::Sprite> spriteDeathDir_;

};

