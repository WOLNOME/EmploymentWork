#pragma once
#include <TextTextureManager.h>
#include <Sprite.h>
#include <string>
#include <list>
#include <array>
#include <Handle.h>
#include <JsonUtil.h>

class MessageUI {
private://構造体
	struct MessageData {
		std::wstring text;			// 全文テキスト
		std::wstring currentText;	// 現在の入力中のテキスト
		Handle textHandle;			// テキストのハンドル
		float inputTimer;			// 入力タイマー
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
	void AddMessage(const std::wstring& text);

private://非公開メンバ関数
	//メッセージの削除処理
	void DeleteMessage();

	// メッセージの更新処理
	void UpdateMessage();

	// スプライトの更新処理
	void UpdateSprite();

private:
	//パラメーター
	json param_;

	//メッセージのコンテナ
	std::list<MessageData> messages_;

	//スプライト
	static const int kMaxSpriteNum_ = 5;	//最大スプライト数
	std::array<Sprite, kMaxSpriteNum_> sprites_;

	//基本のテキストパラメーター
	TextParam baseTextParam_;
	EdgeParam baseEdgeParam_;
};

