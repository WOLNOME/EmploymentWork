#pragma once
#include "GameCamera.h"
#include "Particle.h"
#include "Vector3.h"
#include <MyMath.h>
#include <memory>

//アプリケーション


/// <summary>
/// プレイヤーの死亡演出処理
/// </summary>
class DeathDirection {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// デバッグ処理 
	/// </summary>
	void DebugWithImGui();

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// 演出フラグの取得
	/// </summary>
	/// <returns>演出フラグ</returns>
	bool GetIsDirection() { return isDirection_; }
	/// <summary>
	/// 演出終了フラグの取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDirFinished() { return isDirFinished_; }


	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// ゲームカメラの設定
	/// </summary>
	/// <param name="_gameCamera">ゲームカメラ</param>
	void SetGameCamera(GameCamera* _gameCamera) { camera_ = _gameCamera; }
	/// <summary>
	/// 演出フラグの設定
	/// </summary>
	/// <param name="_isDirection">演出フラグ</param>
	void SetIsDirection(bool _isDirection) { isDirection_ = _isDirection; }

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// カメラの操作
	/// </summary>
	void CameraControl();
	/// <summary>
	/// パーティクル演出
	/// </summary>
	void ParticeDir();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//ゲームカメラ
	GameCamera* camera_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//パーティクル
	std::unique_ptr<Particle> particle_ = nullptr;

	//演出のフラグ
	bool isDirection_ = false;
	bool isDirFinished_ = false;
	//演出のタイマー
	float timer_ = 0.0f;

	//カメラの速度
	Vector3 velocity_ = {};
	//カメラの回転速度
	Vector3 rotateVelocity_ = {};

	//定数グループ
	const float time_ = 3.0f;	//演出の時間
	const float gravity_ = 33.0f;	//重力	
	const float floorHeight_ = 3.0f;	//床の高さ
	const float restitution_ = 0.7f;	//床の反発値
	const float rotateDecay_ = 0.5f;	//回転の減衰値



};

