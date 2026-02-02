#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "CombinedParticle.h"
#include "BulletTrail.h"
#include "JsonUtil.h"
#include <Vector3.h>
#include <memory>

//前方宣言
class PlayerUI;

/// <summary>
/// 敵の使う機関銃弾単体の処理全般を管理するクラス
/// </summary>
class EnemyBullet : public BaseCharacter {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// デストラクタ
	/// </summary>
	~EnemyBullet() override {};
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
	void Spawn(const Vector3& _initPos, const Vector3& _targetPos);

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="attribute">相手の属性</param>
	/// <param name="subjectPos">相手の座標</param>
	void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) override;

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
	/// <summary>
	/// 死亡処理を行う
	/// </summary>
	void DeadProcess();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//プレイヤーUI
	PlayerUI* playerUI_ = nullptr;
	//トレールエフェクト
	std::unique_ptr<BulletTrail> trail_ = nullptr;
	//衝突エフェクト
	std::unique_ptr<CombinedParticle> hitEffect_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメータ
	json param_;

	Vector3 generatedPosition_{};	//生成された座標
	float lifeTimer_;		//寿命タイマー

};

