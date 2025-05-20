#pragma once
#include "application/object/baseCharacter/BaseCharacter.h"
#include "BaseCamera.h"
#include "SceneLight.h"
#include "Input.h"
#include "Object3d.h"
#include "MyMath.h"
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

private://非公開関数
	//移動
	void Move();
	//攻撃
	void Attack();
	//弾の更新
	void UpdateBullets();

	//カメラ処理
	void CameraAlgorithm();

private:
	//インプット
	Input* input_ = nullptr;

private:
	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

private:
	//変数
	const float speed_ = 3.0f;						//移動スピード
	const float maxSpeed_ = 50.0f;					//最大移動スピード
	const float rotateSpeed_ = 1.0f / 2.0f * pi;	//回転スピード

};

