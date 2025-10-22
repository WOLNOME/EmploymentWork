#pragma once
#include <application/object/character/base/BaseCharacter.h>
#include <Particle.h>
#include <string>
#include <Vector3.h>
#include <JsonUtil.h>

/// <summary>
/// アイテム単体の処理全般を管理するクラス
/// </summary>
class Item : public BaseCharacter {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Item() override = default;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& _initPos);
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui() override;

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// 当たり判定処理
	/// </summary>
	/// <param name="attribute">相手の属性</param>
	/// <param name="subjectPos">相手の座標</param>
	void OnCollision(CollisionAttribute attribute, const Vector3& subjectPos) override;

	/// <summary>
	/// 死亡までの処理
	/// </summary>
	void UntilDeathProcess();
	/// <summary>
	/// パーティクルの更新
	/// </summary>
	void UpdateParticle();

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パラメーター
	json param_;

	//パーティクル
	std::unique_ptr<Particle> idleParticle_ = nullptr;
	std::unique_ptr<Particle> getParticle_ = nullptr;

	float swingTimer_ = 0.0f; // アイテムの振り子のタイマー
	bool isUp_ = true; // アイテムの上下移動フラグ
};

