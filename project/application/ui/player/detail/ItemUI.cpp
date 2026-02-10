#include "ItemUI.h"
#include <WinApp.h>
#include <TextureManager.h>
#include <SpriteManager.h>
#include <cassert>

//アプリケーション
#include <application/object/character/player/Player.h>

using namespace Norm;

void ItemUI::Initialize() {
	//パラメーターの初期化
	param_ = JsonUtil::GetJsonData("Resources/parameters/player");
	
}

void ItemUI::Update() {
	//プレイヤーがセットされていなければ警告
	if (!player_) {
		assert(0 && "プレイヤーがセットされていません");
	}

}

void ItemUI::AttachShake(const Vector2& _shakeOffset) {
	_shakeOffset;
}

void ItemUI::AttachBlinking(const Vector4& _color) {
	_color;
}
