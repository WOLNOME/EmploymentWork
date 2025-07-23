#include "MessageUI.h"
#include <MyMath.h>

void MessageUI::Initialize() {
	//基本のテキストパラメーターを設定
	baseTextParam_.text = L"";
	baseTextParam_.font = Font::UDDegitalNP_B;
	baseTextParam_.fontStyle = FontStyle::Normal;
	baseTextParam_.size = 32.0f;
	baseTextParam_.color = { 0.8f,0.8f,0.0f,1.0f };
	//基本のエッジパラメーターを設定
	baseEdgeParam_.width = 2;
	baseEdgeParam_.isEdgeDisplay = 1;
	baseEdgeParam_.slideRate = { 0.0f,0.0f };
	baseEdgeParam_.color = { 0.8f,0.0f,0.0f,1.0f };
}

void MessageUI::Update() {
	// メッセージが無い場合は何もしない
	if (messages_.empty()) return;

	// メッセージの削除
	DeleteMessage();

	// メッセージの更新
	UpdateMessage();
}

void MessageUI::DebugWithImGui() {
#ifdef _DEBUG
#endif // _DEBUG
}

void MessageUI::AddMessage(const std::wstring& text, float inputTime, float displayTime) {
	MessageData newMessage;
	newMessage.text = text;
	newMessage.currentText = L"";
	newMessage.inputTime = inputTime;
	newMessage.inputTimer = inputTime;
	newMessage.displayTime = displayTime;
	newMessage.displayTimer = displayTime;
	newMessage.isInput = true;
	newMessage.isFinished = false;
	newMessage.textHandle = TextTextureManager::GetInstance()->LoadTextTexture(baseTextParam_);
	TextTextureManager::GetInstance()->EditEdgeParam(newMessage.textHandle, baseEdgeParam_);
	messages_.push_back(newMessage);
}

void MessageUI::DeleteMessage() {
	// 終了判定の出たメッセージをリストから削除
	for (auto it = messages_.begin(); it != messages_.end();) {
		if (it->isFinished) {
			it = messages_.erase(it); // メッセージを削除
		}
		else {
			++it; // 次のメッセージへ
		}
	}
}

void MessageUI::UpdateMessage() {
	for (auto& message : messages_) {
		//入力中のメッセージ処理
		if (message.isInput) {
			message.inputTimer -= kDeltaTime;
			if (message.inputTimer <= 0.0f) {
				//表示するテキストを更新
				TextTextureManager::GetInstance()->EditTextString(message.textHandle, message.text);

				message.inputTimer = 0.0f;
				message.isInput = false; // 入力が完了

				continue;	//次のメッセージへ
			}
			// 入力中のテキストを更新
			message.currentText = message.text.substr(0, static_cast<size_t>(message.text.size() * (message.inputTimer / message.inputTime)));
			TextTextureManager::GetInstance()->EditTextString(message.textHandle, message.currentText);
		}
		//表示中のメッセージ処理
		else {
			message.displayTimer -= kDeltaTime;
			if (message.displayTimer <= 0.0f) {
				message.displayTimer = 0.0f;
				message.isFinished = true;	// 終了

				continue;	// 次のメッセージへ
			}

			// 透明にしていく
			float alpha = message.displayTimer / message.displayTime;
			TextTextureManager::GetInstance()->EditTextColor(message.textHandle, { baseTextParam_.color.x, baseTextParam_.color.y, baseTextParam_.color.z, alpha });
			TextTextureManager::GetInstance()->EditEdgeColor(message.textHandle, { baseEdgeParam_.color.x, baseEdgeParam_.color.y, baseEdgeParam_.color.z, alpha });
		}
	}
}
