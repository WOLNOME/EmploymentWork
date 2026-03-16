#pragma once
#include <application/object/character/base/BaseCharacter.h>
#include <CombinedParticle.h>
#include <string>
#include <Vector3.h>
#include <JsonUtil.h>

/// <summary>
/// チュートリアル収集物を管理するクラス
/// </summary>
class ItemTutorialCollectible : public BaseCharacter {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ItemTutorialCollectible() override = default;
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
	/// <param name="initPos_">初期位置</param>
	void Spawn(const Norm::Vector3& _initPos);

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// 静止パーティクルの取得
	/// </summary>
	/// <returns>静止パーティクル</returns>
	Norm::CombinedParticle* GetIdleParticle() { return idleParticle_.get(); }
	/// <summary>
	/// 取得パーティクルの取得
	/// </summary>
	/// <returns>取得パーティクル</returns>
	Norm::CombinedParticle* GetGetParticle() { return getParticle_.get(); }

private:
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
	std::unique_ptr<Norm::CombinedParticle> idleParticle_ = nullptr;
	std::unique_ptr<Norm::CombinedParticle> getParticle_ = nullptr;

	float swingTimer_ = 0.0f; // アイテムの振り子のタイマー
	bool isUp_ = true; // アイテムの上下移動フラグ

};

