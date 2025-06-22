#pragma once
#include "application/object/baseCharacter/BaseCharacter.h"
#include "GameCamera.h"
#include "SceneLight.h"
#include "Object3d.h"
#include "MyMath.h"
#include "Particle.h"
#include "JsonUtil.h"
#include <memory>

class Player;
class Boss : public BaseCharacter {
public:
	//デストラクタ
	~Boss() override {};
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
	/// <summary>
	/// ライン描画
	/// </summary>
	void DrawLine() override;

	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui() override;

private:
	//当たり判定処理
	void OnCollision(CollisionAttribute attribute, Vector3 subjectWorldPos) override;

public://getter
	int GetHP() const { return hp_; }
	int GetMaxHP() const { return maxHP_; }
	bool GetIsCannonFire() const { return isCannonFire_; }

public://setter
	void SetPlayer(Player* _player) { player_ = _player; }
	void SetPosition(const Vector3& _pos);

private://非公開関数
	//移動
	void Move();
	//回転
	void Rotate();
	//攻撃
	void Attack();
	//死亡処理
	void DeadProcess();

private:
	//プレイヤー
	Player* player_ = nullptr;

private://メンバ変数
	//パラメータ
	json param_;

	//砲弾攻撃
	float cannonCoolTime_;
	float cannonCoolTimer_;
	bool isCannonFire_ = false;

	//HP
	int maxHP_;			//最大HP
	int hp_ = maxHP_;	//現在のHP

	//ボス専用テクスチャ
	uint32_t textureHandle_;

};

