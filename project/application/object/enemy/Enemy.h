#pragma once
#include "BaseCamera.h"
#include "SceneLight.h"
#include "Object3d.h"
#include "MyMath.h"
#include <memory>

#include "application/object/player/Player.h"

class Enemy {
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
	void Draw();

	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui();

public://setter
	void SetCamera(BaseCamera* _camera) { camera_ = _camera; }
	void SetSceneLight(SceneLight* _light) { light_ = _light; if (object3d_) object3d_->SetSceneLight(light_); }
	void SetPlayer(Player* _player) { player_ = _player; }

private://非公開関数
	//移動
	void Move();
private:
	//カメラ
	BaseCamera* camera_ = nullptr;
	//シーンライト
	SceneLight* light_ = nullptr;
	//プレイヤー
	Player* player_ = nullptr;

private:
	//オブジェクト
	std::unique_ptr<Object3d> object3d_ = nullptr;

private:
	//変数
	Vector3 velocity_ = {};							//速度
	const float speed_ = 2.5f;						//移動スピード
	const float maxSpeed_ = 20.0f;					//最大移動スピード
	const float rotateSpeed_ = 1.0f / 3.0f * pi;	//回転スピード
	const float floorRegist_ = 50.0f;				//床の摩擦力
	const float searchPlayerDistance_ = 250.0f;		//プレイヤー探索距離

};

