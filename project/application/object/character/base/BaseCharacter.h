#pragma once
#include "Collider.h"
#include "GameCamera.h"
#include "SceneLight.h"
#include "Object3d.h"

//キャラクター基底クラス
class BaseCharacter : public Collider {
public:
	//コンストラクタ
	BaseCharacter() = default;
	//デストラクタ
	virtual ~BaseCharacter() = default;
	//初期化
	virtual void Initialize();
	//更新
	virtual void Update();
	//描画
	virtual void Draw();

	//デバッグ用パラメーター調整
	virtual void DebugWithImGui();

public://getter
	//ワールドトランスフォームの取得
	const WorldTransform& GetWorldTransform() { return object3d_->worldTransform; }
	//コライダー用のワールド座標を取得
	Vector3 GetWorldPosition() override { return object3d_->worldTransform.worldTranslate; }
	//コライダー用前フレーム座標を取得
	Vector3 GetPrePosition() override { return prePosition_; }

	//死亡フラグの取得
	bool GetIsDead() const { return isDead_; }
	//死亡タイマーの取得
	float GetDeadTimer() const { return deadTimer_; }

public://setter
	//カメラのセット
	void SetCamera(GameCamera* _camera) { camera_ = _camera; }
	//シーンライトのセット
	void SetSceneLight(SceneLight* _light) { light_ = _light; if (object3d_) object3d_->SetSceneLight(light_); }
	//死亡予約関数
	void SetDeadTimer(float remainingSeconds);


protected://インスタンス
	//カメラ
	GameCamera* camera_ = nullptr;
	//シーンライト
	SceneLight* light_ = nullptr;
protected://オブジェクト
	//テクスチャハンドル
	int32_t textureHandle_ = EOF;
	//オブジェクト3D
	std::unique_ptr<Object3d> object3d_ = nullptr;
	//前フレームの座標
	Vector3 prePosition_ = {};

protected://キャラ共有のメンバ変数
	//速度
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };
	//死亡フラグ
	bool isDead_ = false;

	//重力値
	float gravity_ = 33.0f;
	//空気抵抗値
	float airResistance_ = 1.0f;
	//床の摩擦値
	float floorFriction_ = 40.0f;

private://死亡予約用変数
	float deadTimer_ = 0.0f;	//死亡までの時間
};

