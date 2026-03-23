#pragma once
#include <cstdint>
#include <Vector3.h>

/// 前方宣言
class CameraManager;

/// <summary>
/// カメラ制御システム
/// </summary>
class CameraSystem {
private:
	/// ============================== ///
	///		列挙体
	/// ============================== ///

	/// <summary>
	/// 流れ
	/// </summary>
	enum class Flow {
		Idle,			//未稼働
		StartBlackOut,	//暗転（始め）
		MoveCamera,		//カメラ移動
		Stillness,		//静止
		EndBlackOut		//暗転（終わり）
	};



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

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// 演出中かの取得
	/// </summary>
	/// <returns></returns>
	bool GetIsDirection() const { return isDirection_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// CameraManagerの設定
	/// </summary>
	/// <param name="manager">借用するCameraManager</param>
	void SetCameraManager(CameraManager* manager) { cameraManager_ = manager; }

	/// <summary>
	/// ターゲットトランスフォームの設定
	/// </summary>
	void SetTargetTransform(Norm::Vector3 _pos, Norm::Vector3 _rot){}

private:
	/// ============================== ///
	///		内部処理
	/// ============================== ///


private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	CameraManager* cameraManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//演出全体
	bool isDirection_ = false;
	Flow flow_ = Flow::Idle;

	//カメラ制御
	float moveTimer_ = 0.0f;
	float moveDuration_ = 5.0f;

	Norm::Vector3 startPos_ = {};
	Norm::Vector3 startRot_ = {};
	Norm::Vector3 targetPos_ = {};
	Norm::Vector3 targetRot_ = {};

	//暗転
	float halfBlackOutTimer_ = 0.0f;
	float halfBlackOutDuration_ = 0.5f;
	bool isOut_ = false;

	//静止
	float stillnessTimer_ = 0.0f;
	float stillnessDuration_ = 2.0f;

};
