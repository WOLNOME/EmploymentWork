#pragma once
#include "BaseScene.h"
#include "DevelopCamera.h"

//アプリケーション
#include "application/object/environment/Skydome.h"
#include "application/object/environment/Ground.h"
#include <application/system/TitleSystem.h>

class TitleScene : public BaseScene {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 終了時
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
	/// <summary>
	/// デバッグ処理
	/// </summary>
	void DebugWithImGui() override;

private:
	//インプット
	Input* input_ = nullptr;

	//ゲーム用カメラ
	std::unique_ptr<DevelopCamera> camera_ = nullptr;
private://ライト
	
private://システム
	std::unique_ptr<TitleSystem> titleSystem_ = nullptr;


};

