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
	// 初期化
	virtual void Initialize();
	// 更新
	virtual void Update();
	// 描画
	virtual void Draw() = 0;
	// ライン描画処理
	virtual void DrawLine();

	// デバッグ用パラメーター調整
	virtual void DebugWithImGui() = 0;

public:
	// ワールドトランスフォームの取得
	const WorldTransform& GetWorldTransform() { return object3d_->worldTransform; }
	// コライダー用のワールド座標を取得
	Vector3 GetWorldPosition() override { return object3d_->worldTransform.worldTranslate; }

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
	// テクスチャハンドル
	int32_t textureHandle_ = EOF;
	// オブジェクト3D
	std::unique_ptr<Object3d> object3d_ = nullptr;

protected://キャラ共有のメンバ変数
	// 速度
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };
	//死亡フラグ
	bool isDead_ = false;

	//重力値
	float gravity_ = 98.0f;
	//空気抵抗値
	float airResistance_ = 1.0f;
	//床の摩擦値
	float floorFriction_ = 40.0f;
};

