#pragma once
#include "BaseScene.h"
class GameOverScene : public BaseScene {
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

private:
	//インプット
	Input* input_ = nullptr;

	//ゲーム用カメラ
	//std::unique_ptr<GameCamera> camera_ = nullptr;
private://ライト
	//平行光源
	//std::unique_ptr<DirectionalLight> dirLight_;
private://オブジェクト

	//テキストテクスチャハンドル
	Handle thGameOverText_;
	//スプライト
	std::unique_ptr<Sprite> spriteGameOverText_ = nullptr;

};

