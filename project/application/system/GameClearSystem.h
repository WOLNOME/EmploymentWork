#pragma once
#include <Input.h>
#include <GameCamera.h>
#include <SceneManager.h>
#include <Object3d.h>
#include <Sprite.h>
#include <vector>
#include <memory>
#include <Handle.h>

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
		 float charRotateDuration = 1.2f;			//一const文字が回転する時間
		float timer = 0.0f;					//タイマー
	};
	/// <summary>
	/// カメラワーク用パラメーター構造体
	/// </summary>
	struct CameraWorkParam {
		Vector3 startPos = { -5.0f,4.0f,-5.0f };		//カメラ開始位置
		Vector3 endPos = { -20.0f,1.0f,-20.0f };		//カメラ終了位置
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
	/// ゲームカメラのセット
	/// </summary>
	/// <param name="_gameCamera">ゲームカメラ</param>
	void SetGameCamera(GameCamera* _gameCamera) { gameCamera_ = _gameCamera; }

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

	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//インプット
	Input* input_ = nullptr;
	//シーンマネージャー
	SceneManager* sceneManager_ = nullptr;
	//ゲームカメラ
	GameCamera* gameCamera_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///
	
	//戦車オブジェクト
	std::unique_ptr<Object3d> tank_ = nullptr;

	//クリアテキスト
	std::vector<Handle> clearTextHandles_;
	std::vector<std::unique_ptr<Sprite>> clearTextSprites_;

	//タイトルに戻るテキスト
	Handle titleTextHandle_;
	std::unique_ptr<Sprite> titleTextSprite_ = nullptr;

	//テキスト操作変数
	TextDirectionParam textDirectionParam_;

	//カメラワーク変数
	CameraWorkParam cameraWorkParam_;

};

