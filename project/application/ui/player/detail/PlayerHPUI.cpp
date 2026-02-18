#include "PlayerHPUI.h"
#include <TextureManager.h>
#include <SpriteManager.h>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>

using namespace Norm;

void PlayerHPUI::Initialize() {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/player");
	//スプライトの初期化
	
}

void PlayerHPUI::Update() {
	//プレイヤーがセットされていなければ警告
	if (!player_) {
		assert(0 && "プレイヤーがセットされていません");
	}

}

void PlayerHPUI::AttachShake(const Vector2& _shakeOffset) {
	//スプライトにシェイクを適用する
	_shakeOffset;
}

void PlayerHPUI::AttachBlinking(const Vector4& _color) {
	//スプライトに点滅を適用する
	_color;
}