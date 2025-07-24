#include "MessageUI.h"
#include <WinApp.h>
#include <SpriteManager.h>
#include <MyMath.h>

void MessageUI::Initialize() {
	//パラメーターの読み込み
	param_ = JsonUtil::GetJsonData("Resources/parameters/messageUI");

	//スプライトの生成・初期化
	for (int i = 0; i < kMaxSpriteNum_; ++i) {
		sprites_[i].Initialize(SpriteManager::GetInstance()->GenerateName("messageUI"), Sprite::Order::Front1);
		sprites_[i].SetIsDisplay(false);
		sprites_[i].SetAnchorPoint({ 0.5f, 0.5f });
	}

	//基本のテキストパラメーターを設定
	auto textColor = param_["textColor"];
	baseTextParam_.text = L"";
	baseTextParam_.font = (Font)param_["textFont"];
	baseTextParam_.fontStyle = (FontStyle)param_["textFontStyle"];
	baseTextParam_.size = param_["textSize"];
	baseTextParam_.color = Vector4(
		textColor[0].get<float>(),
		textColor[1].get<float>(),
		textColor[2].get<float>(),
		textColor[3].get<float>()
	);
	//基本のエッジパラメーターを設定
	auto edgeSlideRate = param_["edgeSlideRate"];
	auto edgeColor = param_["edgeColor"];
	baseEdgeParam_.width = param_["edgeWidth"];
	baseEdgeParam_.isEdgeDisplay = 1;
	baseEdgeParam_.slideRate = Vector2(
		edgeSlideRate[0].get<float>(),
		edgeSlideRate[1].get<float>()
	);
	baseEdgeParam_.color = Vector4(
		edgeColor[0].get<float>(),
		edgeColor[1].get<float>(),
		edgeColor[2].get<float>(),
		edgeColor[3].get<float>()
	);
}

void MessageUI::Update() {
	// メッセージが無い場合は何もしない
	if (messages_.empty()) return;

	// メッセージの削除
	DeleteMessage();

	// メッセージの更新
	UpdateMessage();

	//スプライトの更新
	UpdateSprite();
}

void MessageUI::DebugWithImGui() {
#ifdef _DEBUG
#endif // _DEBUG
}

void MessageUI::AddMessage(const std::wstring& text) {
	MessageData newMessage;
	newMessage.text = text;
	newMessage.currentText = L"";
	newMessage.inputTimer = param_["inputTime"];
	newMessage.displayTimer = param_["displayTime"];
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
			float inputTime = param_["inputTime"];
			message.currentText = message.text.substr(0, static_cast<size_t>(message.text.size() * (1.0f - message.inputTimer / inputTime)));
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
			float displayTime = param_["displayTime"];
			float alpha = message.displayTimer / displayTime;
			TextTextureManager::GetInstance()->EditTextColor(message.textHandle, { baseTextParam_.color.x, baseTextParam_.color.y, baseTextParam_.color.z, alpha });
			TextTextureManager::GetInstance()->EditEdgeColor(message.textHandle, { baseEdgeParam_.color.x, baseEdgeParam_.color.y, baseEdgeParam_.color.z, alpha });
		}
	}
}

void MessageUI::UpdateSprite() {
	int index = 0;
	for (auto it = messages_.rbegin(); it != messages_.rend(); ++it) {
		if (index >= kMaxSpriteNum_) break;

		sprites_[index].SetIsDisplay(true);
		sprites_[index].SetPosition(Vector2(WinApp::kClientWidth / 2.0f, WinApp::kClientHeight / 2.0f - 240.0f + index * 35.0f)); // 上から下に並べる
		sprites_[index].SetTexture(it->textHandle);

		++index;
	}

	// 残りのスプライトを非表示にする
	for (; index < kMaxSpriteNum_; ++index) {
		sprites_[index].SetIsDisplay(false);
	}
}
