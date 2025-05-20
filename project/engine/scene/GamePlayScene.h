#pragma once
#include "BaseScene.h"
#include "DevelopCamera.h"
#include "application/object/Skydome.h"
#include "application/object/Ground.h"
#include "application/object/player/Player.h"
#include "application/object/enemy/Enemy.h"


class GamePlayScene : public BaseScene {
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
	/// 描画
	/// </summary>
	void Draw() override;
	/// <summary>
	/// テキスト描画
	/// </summary>
	void TextDraw() override;

private:
	//インプット
	Input* input_ = nullptr;
	//カメラ
	std::unique_ptr<DevelopCamera> camera_ = nullptr;
private://ライト
	//平行光源
	std::unique_ptr<DirectionalLight> dirLight_;
private://オブジェクト
	//天球
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//地面
	std::unique_ptr<Ground> ground_ = nullptr;

	//プレイヤー
	std::unique_ptr<Player> player_ = nullptr;
	//敵
	std::unique_ptr<Enemy> enemy_ = nullptr;

};

