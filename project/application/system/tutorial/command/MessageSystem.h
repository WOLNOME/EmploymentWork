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
	void ShowText(const std::string& text,bool isAttachNextUI);
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
	Norm::Vector2 centerPos_ = {640,90};

	//ウィンドウのスプライト
	std::unique_ptr<Norm::Sprite> windowSprite_;
	//ウィンドウに必要な変数
	bool isOpenWindow_ = false;
	bool isDirectionWindow_ = false;
	float dirTimer_ = 0.0f;			//表示・非表示に使うタイマー
	float dirDuration_ = 0.1f;		//表示・非表示にかかる時間

	//次に進むUIのスプライト
	std::unique_ptr<Norm::Sprite> nextUISprite_;
	//次に進むUIに必要な変数
	bool isAttachNextUI_ = false;
	float blinkingTimer_ = 0.0f;
	float blinkingDuration_ = 0.5f;

	//テキストのスプライト
	Norm::Handle textHandle_;
	std::unique_ptr<Norm::Sprite> textSprite_;
	//テキストに必要な変数
	bool isDisplayText_ = false;
	float inputTimer_ = 0.0f;		//1文字入力のタイマー
	float inputDuration_ = 0.05f;	//1文字入力にかかる時間
	std::string allMessage_;
	std::string currentMessage_;


};
