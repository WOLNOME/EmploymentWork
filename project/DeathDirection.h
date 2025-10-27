#pragma once
#include "GameCamera.h"
#include "Particle.h"
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
	void CameraContorol();

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

};

