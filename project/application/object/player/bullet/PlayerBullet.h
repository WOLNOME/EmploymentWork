#pragma once
#include "BaseCamera.h"
#include "SceneLight.h"
#include "Object3d.h"

class PlayerBullet {
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

	//ImGuiデバッグ
	void DebugWithImGui();

public://getter
	//ワールドトランスフォームの取得
	const WorldTransform& GetWorldTransform() { return object3d_->worldTransform; }
	
public://setter
	/// <summary>
	/// 最初にセットするパラメーター
	/// </summary>
	/// <param name="_initPos">初期位置</param>
	/// <param name="_initVelocity">初速度</param>
	void SetInitParam(const Vector3& _initPos, const Vector3& _initVelocity) {
		object3d_->worldTransform.translate = _initPos;
		velocity_ = _initVelocity;
	}
	//カメラのセット
	void SetCamera(BaseCamera* _camera) { camera_ = _camera; }
	//シーンライトのセット
	void SetSceneLight(SceneLight* _light) { light_ = _light; if (object3d_) object3d_->SetSceneLight(light_); }

private:
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();
private:
	//カメラ
	BaseCamera* camera_ = nullptr;
	//シーンライト
	SceneLight* light_ = nullptr;

private:
	//オブジェクト
	int32_t textureHandle_ = EOF;
	std::unique_ptr<Object3d> object3d_ = nullptr;

private:
	//変数
	Vector3 velocity_ = {};							//速度
	const float maxSpeed_ = 200.0f;					//最大移動スピード
	const float gravity_ = 98.0f;					//重力
	const float airResistance_ = 0.1f;				//空気抵抗
};

