#pragma once
#include <Input.h>
#include <Audio.h>
#include <CombinedParticle.h>
#include <SceneManager.h>
#include <Object3d.h>
#include <Sprite.h>
#include <vector>
#include <memory>
#include <random>
#include <Handle.h>

//前方宣言
class CameraManager;

/// <summary>
/// ゲームクリアシーンのUIや演出、操作等を管理するクラス
/// </summary>
class GameClearSystem {
private:
	/// ============================== ///
	///		構造体
	/// ============================== ///

	/// <summary>
	/// テキスト演出用パラメーター構造体
	/// </summary>
	struct TextDirectionParam {
		const float allCharRotateStartTime = 1.8f;		//全文字回転開始にかかる時間(X秒までに全て回転開始してる=最後の文字が回転開始する時間)
		float charRotateDuration = 1.2f;			//一文字が回転する時間
		float timer = 0.0f;					//タイマー
	};
	/// <summary>
	/// カメラワーク用パラメーター構造体
	/// </summary>
	struct CameraWorkParam {
		Norm::Vector3 startPos = { -5.0f,4.0f,-5.0f };		//カメラ開始位置
		Norm::Vector3 endPos = { -20.0f,1.0f,-20.0f };		//カメラ終了位置
		float moveDuration = 2.0f;					//移動にかかる時間
		float timer = 0.0f;							//タイマー
		bool isEnd = false;						//カメラワーク終了フラグ
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
	/// カメラマネージャーのセット
	/// </summary>
	/// <param name="_cameraManager">カメラマネージャー</param>
	void SetCameraManager(CameraManager* _cameraManager) { cameraManager_ = _cameraManager; }

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// 操作処理
	/// </summary>
	void Operate();
	/// <summary>
	/// テキスト演出処理
	/// </summary>
	void DirectionUI();
	/// <summary>
	/// カメラワーク処理
	/// </summary>
	void CameraWork();
	/// <summary>
	/// 紙吹雪パーティクル更新処理
	/// </summary>
	void ConfettiParticleUpdate();

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//インプット
	Norm::Input* input_ = nullptr;
	//シーンマネージャー
	Norm::SceneManager* sceneManager_ = nullptr;
	//カメラマネージャー
	CameraManager* cameraManager_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//SE
	std::unique_ptr<Norm::Audio> decideSE_ = nullptr;		//決定

	//ランダムエンジン
	std::mt19937 engine{ std::random_device{}() };

	//戦車オブジェクト
	std::unique_ptr<Norm::Object3d> tank_ = nullptr;
	Norm::WorldTransform tankWorldTransform_;

	//クリアテキスト
	std::vector<Norm::Handle> clearTextHandles_;
	std::vector<std::unique_ptr<Norm::Sprite>> clearTextSprites_;

	//クリアテキスト背景
	std::vector<std::unique_ptr<Norm::Sprite>> clearTextBack_;

	//タイトルに戻るテキスト
	uint32_t titleTextHandle_;
	std::unique_ptr<Norm::Sprite> titleTextSprite_ = nullptr;

	//紙吹雪パーティクル
	std::unique_ptr<Norm::CombinedParticle> confettiParticle_ = nullptr;

	//テキスト操作変数
	TextDirectionParam textDirectionParam_;

	//カメラワーク変数
	CameraWorkParam cameraWorkParam_;

};

