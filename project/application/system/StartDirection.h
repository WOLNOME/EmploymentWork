#pragma once
#include <Input.h>
#include <Sprite.h>
#include <memory>

//アプリケーション
#include <application/ui/message/MessageUI.h>

/// <summary>
/// スタート演出を管理するクラス
/// </summary>
class StartDirection {
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// スタート演出中かどうか取得
	/// </summary>
	/// <returns>スタート演出中かどうか</returns>
	bool GetIsStartDirection()const { return isStartDirection_; }

	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// メッセージUIのセット
	/// </summary>
	/// <param name="_messageUI">メッセージUIのポインタ</param>
	void SetMessageUI(MessageUI* _messageUI) { messageUI_ = _messageUI; }

private:
	/// ============================== ///
	///		インスタンス
	/// ============================== ///

	//インプット
	Norm::Input* input_ = nullptr;

	//メッセージUI
	MessageUI* messageUI_ = nullptr;

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//スタート演出中フラグ
	bool isStartDirection_ = false;
	bool isMaskDisappear_ = false;

	//マスク
	std::unique_ptr<Norm::Sprite> maskSprite_ = nullptr;

	bool isFirstFrame_ = true;
	uint32_t messageId_ = 0u;
};

