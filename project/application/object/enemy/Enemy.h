#pragma once
#include "application/object/baseCharacter/BaseCharacter.h"
#include "BaseCamera.h"
#include "SceneLight.h"
#include "Object3d.h"
#include "MyMath.h"
#include <memory>

#include "application/object/player/Player.h"

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
private:
	//プレイヤー
	Player* player_ = nullptr;

private:
	//変数
	const float speed_ = 2.5f;						//移動スピード
	const float maxSpeed_ = 20.0f;					//最大移動スピード
	const float rotateSpeed_ = 1.0f / 3.0f * pi;	//回転スピード
	const float searchPlayerDistance_ = 250.0f;		//プレイヤー探索距離

};

