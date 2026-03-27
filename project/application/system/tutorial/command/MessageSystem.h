#pragma once
#include <memory>
#include <string>
#include <Vector2.h>
#include <Handle.h>
#include <Sprite.h>

//メッセージ表示を管理するクラス
class MessageSystem {
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

	/// <summary>
	/// メッセージウィンドウを開く
	/// </summary>
	bool OpenWindow();
	/// <summary>
	/// メッセージウィンドウを閉じる
	/// </summary>
	bool CloseWindow();
	/// <summary>
	/// メッセージを表示する
	/// </summary>
	/// <param name="text">テキスト</param>
	/// <param name="isAttachNextUI">次へ進むUIを付けるか</param>
	void ShowText(const std::wstring& text, bool isAttachNextUI);
	/// <summary>
	/// メッセージを削除
	/// </summary>
	void ClearText();

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// ウィンドウの表示状態を取得
	/// </summary>
	/// <returns>ウィンドウの表示状態</returns>
	bool GetIsOpenWindow()const { return isOpenWindow_; }
	/// <summary>
	/// 次のステップへ進んだかを取得
	/// </summary>
	/// <returns>次のステップへ進んだか</returns>
	bool GetIsNextAdvance()const { return isNextAdvance_; }

	bool GetIsWindowClosed() const { return isWindowClosed_; }

	bool GetIsWindowOpened() const { return isWindowOpened_; }


	/// ============================== ///
	///		setter
	/// ============================== ///

	/// <summary>
	/// 次のステップヘ進んだかをセット
	/// </summary>
	/// <param name="isNextAdvance">次のステップへ進んだか</param>
	void SetIsNextAdvance(bool isNextAdvance) { isNextAdvance_ = isNextAdvance; }

private:
	/// ============================== ///
	///		メンバ関数（private）
	/// ============================== ///

	/// <summary>
	/// テキストウィンドウの更新
	/// </summary>
	void UpdateWindow();
	/// <summary>
	/// テキストの更新
	/// </summary>
	void UpdateText();


	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

	//中央位置
	Norm::Vector2 centerPos_ = { 640,90 };

	//ウィンドウのスプライト
	std::unique_ptr<Norm::Sprite> windowSprite_;
	//ウィンドウに必要な変数
	bool isOpenWindow_ = false;
	bool isWindowOpened_ = false;
	bool isWindowClosed_ = false;
	bool isDirectionWindow_ = false;
	float dirTimer_ = 0.0f;			//表示・非表示に使うタイマー
	float dirDuration_ = 0.4f;		//表示・非表示にかかる時間

	//次に進むUIのスプライト
	std::unique_ptr<Norm::Sprite> nextUISprite_;
	//次に進むUIに必要な変数
	bool isAttachNextUI_ = false;
	bool isNextAdvance_ = false;
	float blinkingTimer_ = 0.0f;
	float blinkingDuration_ = 1.0f;

	//テキストのスプライト
	Norm::Handle textHandle_;
	std::unique_ptr<Norm::Sprite> textSprite_;
	//テキストに必要な変数
	float inputTimer_ = 0.0f;		//1文字入力のタイマー
	float inputDuration_ = 0.06f;	//1文字入力にかかる時間
	std::wstring allMessage_;
	std::wstring currentMessage_;


};
