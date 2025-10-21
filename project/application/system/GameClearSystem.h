#pragma once
#include <Input.h>
#include <SceneManager.h>
#include <Object3d.h>
#include <Sprite.h>
#include <memory>
#include <Handle.h>

/// <summary>
/// ゲームクリアシーンのUIや演出、操作等を管理するクラス
/// </summary>
class GameClearSystem {
public:
	//初期化
	void Initialize();
	//更新
	void Update();

	//デバッグ用
	void DebugWithImGui();

private://非公開メンバ関数
	//操作
	void Operate();
	//UI演出
	void DirectionUI();

private:
	//インプット
	Input* input_ = nullptr;
	//シーンマネージャー
	SceneManager* sceneManager_ = nullptr;

private:
	//テキスト操作変数
	bool isHalfPeriod_ = false;
	float timer_ = 0.0f;
	const float time_ = 2.0f;

	//クリアテキスト
	Handle clearTextHandle_;
	std::unique_ptr<Sprite> clearTextSprite_ = nullptr;

	//タイトルに戻るテキスト
	Handle titleTextHandle_;
	std::unique_ptr<Sprite> titleTextSprite_ = nullptr;

};

