#pragma once
#include "application/object/baseCharacter/BaseCharacter.h"
#include "GameCamera.h"
#include "SceneLight.h"
#include "Input.h"
#include "Object3d.h"
#include "Sprite.h"
#include "MyMath.h"
#include "JsonUtil.h"
#include <list>
#include <memory>

//アプリケーション
#include "application/object/player/bullet/PlayerBullet.h"

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

private://非公開関数
	//回転
	void Rotate();
	//移動
	void Move();
	//攻撃
	void Attack();
	//弾の更新
	void UpdateBullets();
	//死亡処理
	void DeadProcess();

	//カメラ処理
	void CameraAlgorithm();

private:
	//インプット
	Input* input_ = nullptr;

private:
	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

private:
	//パラメータ
	json param_;

	//変数
	int maxHP_;	//最大HP
	int hp_;	//現在のHP
};

