#pragma once
#include "BaseCamera.h"
#include "SceneLight.h"
#include "Input.h"
#include "WorldTransform.h"
#include "Object3d.h"
#include "MyMath.h"
#include <list>
#include <memory>

//アプリケーション
#include "application/object/player/bullet/PlayerBullet.h"

class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(SceneLight* _light);

	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui();

public: //getter
	//ワールドトランスフォームの取得
	const WorldTransform& GetWorldTransform() { return worldTransform_; }
	
public: //setter
	void SetCamera(BaseCamera* _camera) { camera_ = _camera; }

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
	//カメラ
	BaseCamera* camera_ = nullptr;

private:
	//オブジェクト
	WorldTransform worldTransform_;
	std::unique_ptr<Object3d> object3d_ = nullptr;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

private:
	//変数
	Vector3 velocity_ = {};							//速度
	const float speed_ = 3.0f;						//移動スピード
	const float maxSpeed_ = 50.0f;					//最大移動スピード
	const float rotateSpeed_ = 1.0f / 2.0f * pi;	//回転スピード
	const float floorRegist_ = 50.0f;				//床の摩擦力

};

