#include "DecorativeUI.h"
#include <WinApp.h>
#include <TextureManager.h>
#include <SpriteManager.h>
#include <cassert>
#include <PostEffectManager.h>

using namespace Norm;

DecorativeUI::~DecorativeUI() {
	//ポストエフェクトを基に戻す
	PostEffectManager::GetInstance()->SetPostEffect(PostEffectKind::None);
}

void DecorativeUI::Initialize() {
	
}

void DecorativeUI::Update() {
	//プレイヤーインスタンスのチェック
	assert(player_ != nullptr && "プレイヤーインスタンスがセットされていません");

}

void DecorativeUI::AttachShake(const Vector2& _shakeOffset) {
	
}

void DecorativeUI::AttachBlinking(const Vector4& _color) {
	
}