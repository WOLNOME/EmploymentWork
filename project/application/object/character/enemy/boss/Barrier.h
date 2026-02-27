#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "JsonUtil.h"
#include <CombinedParticle.h>
#include <array>
#include <memory>

//前方宣言
class Boss;

/// <summary>
/// バリアクラス
/// </summary>
class Barrier : public BaseCharacter {
private:
	/// ============================== ///
	///		定数
	/// ============================== ///

	static const int kScrapeNum = 50;
public:
	/// ============================== ///
	///		メンバ関数（public）
	/// ============================== ///

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Barrier() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Barrier() override;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// デバッグ用パラメータ調整
	/// </summary>
	void DebugWithImGui() override;

	/// <summary>
	/// スポーン処理
	/// </summary>
	/// <param name="_position">座標</param>
	void Spawn(const Norm::Vector3& _position);


	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// パラメーターの取得
	/// </summary>
	/// <returns>パラメーター</returns>
	const json& GetParam() { return param_; }

	/// <summary>
	/// バリア削れエフェクト取得
	/// </summary>
	/// <returns>エフェクト配列</returns>
	const std::array<std::unique_ptr<Norm::CombinedParticle>, kScrapeNum>& GetScrapes() const {
		return scrapes_;
	}
	/// <summary>
	/// バリア破壊エフェクト取得
	/// </summary>
	/// <returns>破壊エフェクト</returns>
	Norm::CombinedParticle* GetDestroyEffect() const {
		return destroy_.get();
	}

	/// <summary>
	/// HPを取得
	/// </summary>
	/// <returns>HP</returns>
	int GetHP() { return hp_; }
	/// <summary>
	/// 最大HPを取得
	/// </summary>
	/// <returns>最大HP</returns>
	int GetMaxHP() { return maxHP_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// ボスのセット
	/// </summary>
	/// <param name="_boss">ボスのポインタ</param>
	void SetBoss(Boss* _boss) { boss_ = _boss; }

	/// <summary>
	/// HPのセット
	/// </summary>
	/// <param name="_hp">HP</param>
	void SetHP(int _hp) { hp_ = _hp; }

private:
	/// ============================== ///
	///		メンバ関数（private）
	/// ============================== ///

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 死亡処理
	/// </summary>
	void DeadProcess();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	Boss* boss_ = nullptr;

	/// ============================== ///
	///		メンバ変数(private)
	/// ============================== ///

	//パラメーター
	json param_;

	//バリアが削れるエフェクト
	std::array<std::unique_ptr<Norm::CombinedParticle>, kScrapeNum> scrapes_;

	//バリアが割れるエフェクト
	std::unique_ptr<Norm::CombinedParticle> destroy_ = nullptr;


	//HP
	int hp_ = 0;
	int maxHP_ = 0;

};

