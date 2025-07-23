#pragma once
#include <TextTextureManager.h>
#include <string>
#include <list>
#include <Handle.h>

class MessageUI {
private://構造体
	struct MessageData {
		std::wstring text;			// 全文テキスト
		std::wstring currentText;	// 現在の入力中のテキスト
		Handle textHandle;			// テキストのハンドル
		float inputTime;			// 入力時間
		float inputTimer;			// 入力タイマー
		float displayTime;			// 表示時間
		float displayTimer;			// 表示タイマー
		bool isInput;				// 入力中かどうか
		bool isFinished;			// 完了したかどうか
	};

public:
	// コンストラクタ
	MessageUI() = default;
	// デストラクタ
	~MessageUI() = default;
	// 初期化
	void Initialize();
	// 更新
	void Update();

	// デバッグ用ImGui
	void DebugWithImGui();

	// メッセージの追加
	void AddMessage(const std::wstring& text, float inputTime, float displayTime);

private://非公開メンバ関数
	//メッセージの削除処理
	void DeleteMessage();

	// メッセージの更新処理
	void UpdateMessage();

private:
	//メッセージのコンテナ
	std::list<MessageData> messages_;

	//基本のテキストパラメーター
	TextParam baseTextParam_;
	EdgeParam baseEdgeParam_;
};

