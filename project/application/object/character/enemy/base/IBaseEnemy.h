#pragma once
#include "application/object/character/base/BaseCharacter.h"
#include "Particle.h"
#include "JsonUtil.h"
#include <memory>

class Player;
class IBaseEnemy : public BaseCharacter {
public:
	// デストラクタ
	~IBaseEnemy() override = default;
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

public://getter
	int GetMaxHP() const { return maxHP_; }
	int GetHP() const { return hp_; }

public://setter
	void SetPlayer(Player* _player) { player_ = _player; }
	void SetPosition(const Vector3& _pos);

protected:
	//当たり判定処理
	void OnCollision(CollisionAttribute attribute) override;

private:
	//移動
	void Move();
	//回転
	void Rotate();
	//死亡処理
	void DeadProcess();

protected://借用インスタンス
	Player* player_ = nullptr;

protected://メンバ変数
	//パラメーター
	json param_;

	//HP
	int maxHP_;	//最大HP
	int hp_;	//現在のHP

	//パーティクル
	std::unique_ptr<Particle> deadParticle_ = nullptr;
	float deadParticleOnTime_ = 0.5f;
	float deadParticleOnTimer_ = 0.0f;

};

