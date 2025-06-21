#pragma once
#include "application/object/baseCharacter/BaseCharacter.h"
#include "GameCamera.h"
#include "SceneLight.h"
#include "Input.h"
#include "Object3d.h"
#include "Sprite.h"
#include "MyMath.h"
#include "JsonUtil.h"
#include <vector>
#include <memory>

class Player : public BaseCharacter {
public:
	//デストラクタ
	~Player() override {};
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

public://getter
	//MaxHP
	int GetMaxHP() const { return maxHP_; }
	//HP
	int GetHP() const { return hp_; }
	//砲弾リロードタイム
	float GetCannonReloadTime() const { return cannonReloadTime_; }
	//砲弾リロードタイムタイマー
	float GetCannonReloadTimer() const { return cannonReloadTimer_; }
	//砲弾を発射したかどうか
	bool GetIsCannonFire() const { return isCannonFire_; }
	//銃弾リロードタイム
	float GetBulletReloadTime() const { return bulletReloadTime_; }
	//銃弾リロードタイムタイマー
	float GetBulletReloadTimer() const { return bulletReloadTimer_; }
	//銃弾を発射したかどうか
	bool GetIsBulletFire() const { return isBulletFire_; }

private://非公開関数
	//回転
	void Rotate();
	//移動
	void Move();
	//キャノン攻撃
	void CannonAttack();
	//銃弾攻撃
	void BulletAttack();
	//死亡処理
	void DeadProcess();

	//カメラ処理
	void CameraAlgorithm();

private:
	//インプット
	Input* input_ = nullptr;

private:
	//パラメータ
	json param_;

	//変数
	int maxHP_;	//最大HP
	int hp_;	//現在のHP

	float cannonReloadTime_;		//砲弾リロードタイム(秒)
	float cannonReloadTimer_;		//砲弾リロードタイム計測用タイマー
	bool isCannonFire_ = false;		//砲弾を発射したかどうか

	float bulletReloadTime_;		//銃弾リロードタイム(秒)
	float bulletReloadTimer_;		//銃弾リロードタイム計測用タイマー
	bool isBulletFire_ = false;		//銃弾を発射したかどうか
};

