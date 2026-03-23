#include "MessageSystem.h"
#include <TextureManager.h>
#include <SpriteManager.h>

using namespace Norm;

void MessageSystem::Initialize() {

	//ウィンドウ用テクスチャを読み込む
	uint32_t textureHandle = TextureManager::GetInstance()->LoadTexture("messageWindow.png");

	//スプライト生成
	windowSprite_ = std::make_unique<Sprite>();
	windowSprite_->Initialize(
		SpriteTag{},
		SpriteManager::GetInstance()->GenerateName("messageWindow"),
		Order::Front5,
		textureHandle
	);

	//テキスト

	//初期状態では非表示
	windowSprite_->SetIsDisplay(false);

	//中央位置の初期化
	centerPos_ = { 640.0f, 540.0f };
}

void MessageSystem::Update() {

	//ウィンドウが開いていない場合は何もしない
	if (!isOpen_) {
		return;
	}

	//ウィンドウの位置を中央に固定
	windowSprite_->SetPosition(centerPos_);

	//テキスト描画位置も中央に合わせる
	//Handle側の座標設定関数がある前提
	textHandle_.SetPosition(centerPos_);
}

void MessageSystem::Open() {

	//すでに開いている場合は何もしない
	if (isOpen_) {
		return;
	}

	isOpen_ = true;

	//スプライトを表示
	windowSprite_->SetIsDisplay(true);
}

void MessageSystem::Close() {

	//すでに閉じている場合は何もしない
	if (!isOpen_) {
		return;
	}

	isOpen_ = false;

	//スプライトを非表示
	windowSprite_->SetIsDisplay(false);

	//表示中のテキストもクリア
	message_.clear();
}

void MessageSystem::ShowMessage(const std::string& text) {

	//メッセージ内容を保存
	message_ = text;

	//Handleに文字列を設定
	textHandle_.SetText(message_);

	//ウィンドウが閉じている場合でもテキストは更新される
}
