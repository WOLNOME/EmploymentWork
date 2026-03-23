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
	void Open();
	/// <summary>
	/// メッセージウィンドウを閉じる
	/// </summary>
	void Close();
	/// <summary>
	/// メッセージを表示する
	/// </summary>
	/// <param name="text">テキスト</param>
	void ShowMessage(const std::string& text);

	/// ============================== ///
	///		getter
	/// ============================== ///

	/// <summary>
	/// ウィンドウの表示状態を取得
	/// </summary>
	/// <returns>ウィンドウの表示状態</returns>
	bool GetIsOpen()const { return isOpen_; }

private:

	//メッセージウィンドウのスプライト
	std::unique_ptr<Norm::Sprite> windowSprite_;

	//テキスト描画用ハンドル
	Norm::Handle textHandle_;
	std::unique_ptr<Norm::Sprite> textSprite_;

	//メッセージ文字列
	float inputTimer_ = 0.0f;
	float inputDuration_ = 1.0f;
	std::string allMessage_;
	std::string currentMessage_;

	//中央位置
	Norm::Vector2 centerPos_;

	//ウィンドウが開いているかどうか
	bool isOpen_ = false;
};
