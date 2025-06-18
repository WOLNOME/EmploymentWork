#pragma once
#include "application/object/baseCharacter/BaseCharacter.h"
#include "GameCamera.h"
#include "SceneLight.h"
#include "Object3d.h"
#include "MyMath.h"
#include "Particle.h"
#include "JsonUtil.h"
#include <list>
#include <memory>

//アプリケーション
#include "application/object/enemy/bullet/EnemyBullet.h"

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
	void OnCollision(CollisionAttribute attribute) override;

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
	//弾の更新処理
	void UpdateBullets();
	//死亡処理
	void DeadProcess();

private:
	//プレイヤー
	Player* player_ = nullptr;
	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

private://メンバ変数
	//パラメータ
	json param_;

	//攻撃
	float attackCoolTimer_ = 0.0f;		//計測用タイマー
	bool isEnemyAttacked_ = false;

	//HP
	int maxHP_;			//最大HP
	int hp_ = maxHP_;	//現在のHP

	//ボス専用テクスチャ
	uint32_t textureHandle_;

};

