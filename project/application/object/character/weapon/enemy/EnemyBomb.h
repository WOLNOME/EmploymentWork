#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "BaseCamera.h"
#include "SceneLight.h"
#include "Object3d.h"
#include "Particle.h"
#include "JsonUtil.h"
#include <Vector3.h>
#include <memory>

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
	/// 当たり判定処理
	/// </summary>
	/// <param name="attribute">相手の属性</param>
	/// <param name="subjectPos">相手の座標</param>
	void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) override;

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// 初期パラメーターを設定する
	/// </summary>
	/// <param name="_initPos">初期位置</param>
	/// <param name="_targetPos">目標位置</param>
	void SetInitParam(const Vector3& _initPos, const Vector3& _targetPos);
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
	/// 爆風処理を行う
	/// </summary>
	void Blast();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//プレイヤーUI
	PlayerUI* playerUI_ = nullptr;

	//パーティクル
	std::unique_ptr<Particle> particle_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメータ
	json param_;

	//生成された座標
	Vector3 generatedPosition_ = {};

	//爆風
	bool isBlast_ = false;
	const float durationTime_ = 1.0f;
	float durationTimer_ = 0.0f;
};

