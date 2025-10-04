#pragma once
#include <Input.h>
#include <SceneManager.h>
#include <Object3d.h>
#include <Sprite.h>
#include <vector>
#include <memory>
#include <Handle.h>

//アプリケーション
#include "application/object/environment/Skydome.h"
#include "application/object/environment/Ground.h"
#include "application/object/character/enemy/manager/EnemyManager.h"

class TitleSystem {
private://列挙体
	//タイトルメニュー
	enum class Menu {
		Start,
		Exit,

		kNumMaxMenu,
	};

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
	
	//タイトルメニュー
	Menu menu_;

	//テキスト操作変数
	bool isHalfPeriod_ = false;
	float timer_ = 0.0f;
	const float time_ = 2.0f;

	//スタートテキスト
	Handle startTextHandle_;
	std::unique_ptr<Sprite> startTextSprite_ = nullptr;


private://オブジェクト
	//ガレージ
	std::unique_ptr<Object3d> garage_ = nullptr;

	//戦車
	std::vector<std::unique_ptr<Object3d>> tanks_;
};

