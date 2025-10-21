#pragma once
#include <TextTextureManager.h>
#include <Sprite.h>
#include <string>
#include <list>
#include <array>
#include <Handle.h>
#include <JsonUtil.h>

/// <summary>
/// メッセージ(通知)UIを管理するクラス
/// </summary>
class MessageUI {
private:
	/// <summary>
	/// メッセージの状態
	/// </summary>
	enum class MessageState {
		Inputting,		// 入力中
		Displaying,		// 表示中
		Disappearing,	// 消滅中
		Finished,		// 完了
	};

	/// <summary>
	/// 1つのメッセージに必要なデータ
	/// </summary>
	struct MessageData {
		uint32_t id;				// メッセージのID
		std::wstring text;			// 全文テキスト
		std::wstring currentText;	// 現在の入力中のテキスト
		Handle textHandle;			// テキストのハンドル
		float inputTimer;			// 入力タイマー
		float displayTimer;			// 表示タイマー
		float disappearTimer;		// 消滅タイマー
		float blinkTimer;			// 点滅タイマー
		MessageState state;			// メッセージの状態
		bool isFinished;			// 完了したかどうか
		bool isBlinking;			// 点滅するかどうか
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
	uint32_t AddMessage(const std::wstring& _text, float _displayTime = 0.0f, bool _isBlinking = false);

	// メッセージの終了通知
	void FinishMessage(uint32_t _messageId);

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
	//メッセージIDカウンター
	uint32_t messageIdCounter_ = 0u;

	//スプライト
	static const int kMaxSpriteNum_ = 5;	//最大スプライト数
	std::array<Sprite, kMaxSpriteNum_> sprites_;

	//基本のテキストパラメーター
	TextParam baseTextParam_;
	EdgeParam baseEdgeParam_;
};

