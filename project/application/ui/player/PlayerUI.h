#pragma once
#include "GameCamera.h"
#include "Sprite.h"
#include "JsonUtil.h"
#include <array>
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

	//デバッグ用ImGui
	void DebugWithImGui();

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
	//HPバー
	std::array<uint32_t, 2> thHPBar_;
	std::array<std::unique_ptr<Sprite>, 2> spriteHPBar_;
	//砲弾UI
	std::array<uint32_t, 2> thCannon_;
	std::array<std::unique_ptr<Sprite>, 2> spriteCannon_;
	//銃弾UI
	std::array<uint32_t, 2> thBullet_;
	std::array<std::unique_ptr<Sprite>, 2> spriteBullet_;


private://データ駆動設計用パラメータ
	json param_;

};

