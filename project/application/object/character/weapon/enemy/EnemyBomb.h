#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "BaseCamera.h"
#include "SceneLight.h"
#include "Object3d.h"
#include "CombinedParticle.h"
#include "JsonUtil.h"
#include <Vector3.h>
#include <memory>

/// <summary>
/// 爆弾の撃ち方
/// </summary>
enum class BombMethod {
	Launch,		//打ち上げ
	Fall		//落下
};

//前方宣言（アプリケーション）
class PlayerUI;

/// <summary>
/// 敵(ジェットエネミー)の使う爆弾の処理全般を管理するクラス
/// </summary>
class EnemyBomb : public BaseCharacter {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBomb() override {};
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui() override;

	/// <summary>
	/// スポーン
	/// </summary>
	/// <param name="_initPos">初期位置</param>
	/// <param name="_targetPos">目標位置</param>
	/// <param name="_size">サイズ</param>
	void Spawn(const BombMethod& _method, const Norm::Vector3& _initPos, const Norm::Vector3& _targetPos, float _size = 1.0f);

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// プレイヤーUIを取得する
	/// </summary>
	/// <returns>プレイヤーUI</returns>
	PlayerUI* GetPlayerUI() { return playerUI_; }
	/// <summary>
	/// 生成された場所を取得する
	/// </summary>
	/// <returns>生成された場所</returns>
	const Norm::Vector3& GetGeneratedPosition() { return generatedPosition_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// プレイヤーUIを設定する
	/// </summary>
	/// <param name="_playerUI">設定するプレイヤーUI</param>
	void SetPlayerUI(PlayerUI* _playerUI) { playerUI_ = _playerUI; }

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// 移動処理を行う
	/// </summary>
	void Move();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//プレイヤーUI
	PlayerUI* playerUI_ = nullptr;

	//爆破範囲警告
	std::unique_ptr<Norm::Object3d> warning_ = nullptr;

	//爆風パーティクル
	std::unique_ptr<Norm::CombinedParticle> explosion_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメータ
	json param_;

	//生成された座標
	Norm::Vector3 generatedPosition_ = {};

};

