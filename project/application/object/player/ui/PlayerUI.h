#pragma once
#include "GameCamera.h"
#include "Sprite.h"
#include <cstdint>

class Player;
class PlayerUI {
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//バックスプライト描画
	void DrawBackSprite();
	//フロントスプライト描画
	void DrawFrontSprite();

public://setter
	void SetPlayer(Player* _player) { player_ = _player; }
	void SetCamera(GameCamera* _camera) { camera_ = _camera; }
private:
	//プレイヤー
	Player* player_ = nullptr;
	//カメラ
	GameCamera* camera_ = nullptr;
private:
	//2dレティクル
	uint32_t th2dReticle_;
	std::unique_ptr<Sprite> sprite2dReticle_ = nullptr;
	//FPSUI
	uint32_t thFPSUI_;
	std::unique_ptr<Sprite> spriteFPSUI_ = nullptr;

private://データ駆動設計用パラメータ
	int shakePower_ = 8;
};

