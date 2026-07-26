#pragma once
#include <CombinedParticle.h>
#include <memory>

//前方宣言
class Player;

/// <summary>
/// ゲームプレイシーンのUIや演出、操作などを管理するクラス
/// </summary>
class GamePlaySystem {
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
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui();

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// プレイヤーのセット
	/// </summary>
	/// <param name="_player"></param>
	void SetPlayer(Player* _player) { player_ = _player; }

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// 灰パーティクルの更新
	/// </summary>
	void AshUpdate();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//プレイヤー
	Player* player_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//灰パーティクル
	std::unique_ptr<Norm::CombinedParticle> ashParticle_ = nullptr;



};

