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

//前方宣言
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

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//プレイヤーUI
	PlayerUI* playerUI_ = nullptr;

	//爆破範囲警告
	std::unique_ptr<Object3d> warning_ = nullptr;

	//爆風パーティクル
	std::unique_ptr<CombinedParticle> explosion_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメータ
	json param_;

	//生成された座標
	Vector3 generatedPosition_ = {};

};

