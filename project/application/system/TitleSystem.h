#pragma once
#include <Input.h>
#include <BaseCamera.h>
#include <SceneManager.h>
#include <Vector3.h>
#include <Object3d.h>
#include <Sprite.h>
#include <vector>
#include <memory>
#include <Handle.h>

//アプリケーション
#include "application/object/environment/Skydome.h"
#include "application/object/environment/Ground.h"
#include "application/object/character/enemy/manager/EnemyManager.h"

/// <summary>
/// タイトルシーンのUIや演出、操作等を管理するクラス
/// </summary>
class TitleSystem {
private:
	/// ============================== ///
	///		列挙体
	/// ============================== ///

	/// <summary>
	/// タイトルメニュー
	/// </summary>
	enum class Menu {
		kStart,
		kExit,

		kNumMaxMenu,
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
	/// <summary>
	/// デバッグ用パラメーター調整
	/// </summary>
	void DebugWithImGui();

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// カメラセット
	/// </summary>
	/// <param name="_camera">カメラのポインタ</param>
	void SetCamera(BaseCamera* _camera) { camera_ = _camera; }

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// 操作処理
	/// </summary>
	void Operate();
	/// <summary>
	/// メニューUI
	/// </summary>
	void DirectionUI();
	/// <summary>
	/// カメラ操作
	/// </summary>
	void OperateCamera();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//インプット
	Input* input_ = nullptr;
	//カメラ
	BaseCamera* camera_ = nullptr;
	//シーンマネージャー
	SceneManager* sceneManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//タイトルメニュー
	Menu menu_;

	//テキスト操作変数
	bool isHalfPeriod_ = false;
	float timer_ = 0.0f;
	const float kTime_ = 2.0f;

	//タイトルテキスト
	Handle titleTextHandle_;
	std::unique_ptr<Sprite> titleTextSprite_ = nullptr;

	//スタートテキスト
	Handle startTextHandle_;
	std::unique_ptr<Sprite> startTextSprite_ = nullptr;

	//ガレージ
	std::unique_ptr<Object3d> garage_ = nullptr;

	//戦車
	std::vector<std::unique_ptr<Object3d>> tanks_;

	//カメラ操作
	Vector3 cameraStartPos_ = { -60.0f,12.0f,-17.0f };
	Vector3 cameraEndPos_ = { 60.0f,12.0f,-17.0f };
	const float kCameraMoveTime_ = 30.0f;
	float cameraTimer_ = 0.0f;
	bool isHalfPeriodCamera_ = false;

	Vector3 cameraTargetPos_ = { 0.0,1.0f,0.0f };
};

