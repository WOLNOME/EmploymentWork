#pragma once
#include <memory>
#include <string>
#include <Vector2.h>
#include <Handle.h>
#include <Sprite.h>

//メッセージ表示を管理するクラス
class MessageSystem {
public:

	//初期化
	void Initialize();

	//毎フレーム更新
	void Update();

	//メッセージウィンドウを開く
	void Open();

	//メッセージウィンドウを閉じる
	void Close();

	//メッセージを表示する
	void ShowMessage(const std::string& text);

	//ウィンドウの表示状態を取得
	bool IsOpen()const;

	//ウィンドウの中央位置を設定
	void SetCenterPosition(const Vector2& pos);

private:

	//メッセージウィンドウのスプライト
	std::unique_ptr<Sprite> windowSprite_;

	//テキスト描画用ハンドル
	Handle textHandle_;

	//現在表示しているメッセージ文字列
	std::string message_;

	//中央位置
	Vector2 centerPos_;

	//ウィンドウが開いているかどうか
	bool isOpen_ = false;
};
