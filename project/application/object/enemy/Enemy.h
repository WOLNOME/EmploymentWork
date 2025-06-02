#pragma once
#include "application/object/baseCharacter/BaseCharacter.h"
#include "BaseCamera.h"
#include "SceneLight.h"
#include "Object3d.h"
#include "MyMath.h"
#include "Particle.h"
#include <list>
#include <memory>

#include "application/object/player/Player.h"
#include "application/object/enemy/bullet/EnemyBullet.h"

class Enemy : public BaseCharacter {
public:
	//デストラクタ
	~Enemy() override {};
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

	//当たり判定処理
	void OnCollision(CollisionAttribute attribute) override;

public://setter
	void SetPlayer(Player* _player) { player_ = _player; }

private://非公開関数
	//移動
	void Move();
	//回転
	void Rotate();
	//攻撃
	void Attack();
	//弾の更新処理
	void UpdateBullets();

private:
	//プレイヤー
	Player* player_ = nullptr;
	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

private://メンバ変数
	//移動
	const float searchPDistanceMove_ = 250.0f;		//プレイヤー探索距離
	const float speed_ = 2.5f;						//移動スピード
	const float maxSpeed_ = 20.0f;					//最大移動スピード

	//回転
	const float searchPDistanceRotate_ = 250.0f;	//プレイヤー探索距離
	const float rotateSpeed_ = 1.0f / 3.0f * pi;	//回転スピード

	//攻撃
	const float searchPDistanceAttack_ = 100.0f;		//プレイヤー探索距離
	const float attackCoolTime_ = 1.5f;				//攻撃のクールタイム
	float attackCoolTimer_ = 0.0f;					//計測用タイマー
	bool isEnemyAttacked_ = false;

};

