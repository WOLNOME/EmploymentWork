#pragma once
#include <Input.h>
#include <Sprite.h>
#include <memory>

//アプリケーション
#include <application/ui/message/MessageUI.h>

class StartDirection {
public:
	//初期化
	void Initialize();
	//更新
	void Update();

	//getter
	bool GetIsStartDirection()const { return isStartDirection_; }

	//setter
	void SetMessageUI(MessageUI* _messageUI) { messageUI_ = _messageUI; }

private:
	//インプット
	Input* input_ = nullptr;

	//メッセージUI
	MessageUI* messageUI_ = nullptr;

	//スタート演出中フラグ
	bool isStartDirection_ = false;
	bool isMaskDisappear_ = false;

	//マスク
	std::unique_ptr<Sprite> maskSprite_ = nullptr;

	bool isFirstFrame_ = true;
	uint32_t messageId_ = 0u;
};

