#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "CombinedParticle.h"
#include "BulletTrail.h"
#include "JsonUtil.h"
#include <Vector3.h>
#include <memory>

//前方宣言（アプリケーション）
class PlayerUI;

/// <summary>
/// 敵の使う砲弾の処理全般を管理するクラス
/// </summary>
class EnemyCannon : public BaseCharacter {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyCannon() override {};
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
	void Spawn(const Norm::Vector3& _initPos, const Norm::Vector3& _targetPos);

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="attribute">相手の属性</param>
	/// <param name="subjectPos">相手の座標</param>
	void OnCollision(Norm::CollisionAttribute attribute, const Norm::Vector3& subjectPos) override;

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
	//トレールエフェクト
	std::unique_ptr<Norm::BulletTrail> trail_ = nullptr;
	//パーティクル
	std::unique_ptr<Norm::CombinedParticle> particle_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメータ-
	json param_;

	//生成された座標
	Norm::Vector3 generatedPosition_ = {};

};

