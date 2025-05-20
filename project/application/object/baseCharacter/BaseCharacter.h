#pragma once
#include "Collider.h"
#include "BaseCamera.h"
#include "SceneLight.h"
#include "Object3d.h"

// キャラクター基底クラス
class BaseCharacter : public Collider {
public:
	// コンストラクタ
	BaseCharacter() = default;
	// デストラクタ
	virtual ~BaseCharacter() = default;
	// 初期化
	virtual void Initialize() = 0;
	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Draw() = 0;
	// デバッグ用パラメーター調整
	virtual void DebugWithImGui() = 0;

public://setter
	// カメラのセット
	void SetCamera(BaseCamera* _camera) { camera_ = _camera; }
	// シーンライトのセット
	void SetSceneLight(SceneLight* _light) { light_ = _light; if (object3d_) object3d_->SetSceneLight(light_); }

protected://インスタンス
	// カメラ
	BaseCamera* camera_ = nullptr;
	// シーンライト
	SceneLight* light_ = nullptr;
protected://オブジェクト
	// オブジェクト3D
	std::unique_ptr<Object3d> object3d_ = nullptr;

protected://キャラ共有のメンバ変数
	// 速度
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };

	//重力値
	float gravity_ = 98.0f;
	//空気抵抗値
	float airResistance_ = 1.0f;
	//床の摩擦値
	float floorFriction_ = 40.0f;
	


};

