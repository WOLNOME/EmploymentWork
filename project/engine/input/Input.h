#pragma once
#include <wrl.h>
#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>
#include "Vector2.h"

///マウスのコマンド
enum MouseButton {
	LeftButton,    // 左ボタン
	RightButton,   // 右ボタン
	MiddleButton   // 中央ボタン
};

///ゲームパッドのコマンド
enum GamepadButton {
	ButtonA,     // Aボタン
	ButtonB,     // Bボタン
	ButtonX,     // Xボタン
	ButtonY,     // Yボタン
	LeftShoulder, // LBボタン
	RightShoulder, // RBボタン
	Back,        // BACKボタン
	Start,       // STARTボタン
	LeftThumb,   // 左スティック押し込み
	RightThumb,  // 右スティック押し込み
	DPadUp,     // 十字キー 上
	DPadDown,   // 十字キー 下
	DPadLeft,   // 十字キー 左
	DPadRight   // 十字キー 右
};

//入力
class Input {
private://コンストラクタ等の隠蔽
	static Input* instance;

	Input() = default;//コンストラクタ隠蔽
	~Input() = default;//デストラクタ隠蔽
	Input(Input&) = delete;//コピーコンストラクタ封印
	Input& operator=(Input&) = delete;//コピー代入演算子封印
public:
	//シングルトンインスタンスの取得
	static Input* GetInstance();
public:
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;//エイリアステンプレート

public://メンバ関数
	//初期化
	void Initialize();
	//更新
	void Update();
	//終了
	void Finalize();

private://非公開メンバ関数
	//ダイレクトインプットの初期化
	void InitDirectInput();
	//マウスデバイスの生成
	void GenerateMouse();
	//キーボードデバイスの生成
	void GenerateKeyboard();
	//ゲームパッドデバイスの生成
	void GenerateGamepad();

public://固有の処理
	bool PushMouseButton(MouseButton button);
	bool TriggerMouseButton(MouseButton button);
	void SetExclusiveMode(bool isExclusive);
	bool PushKey(BYTE keyNumber);
	bool TriggerKey(BYTE keyNumber);
	bool PushPadButton(GamepadButton button);
	bool TriggerPadButton(GamepadButton button);
	Vector2 GetMousePosition();
	Vector2 GetMouseDelta();
	float GetMouseScrollCount();
	Vector2 GetLeftStickDir();
	Vector2 GetRightStickDir();
private://インスタンス

private://メンバ変数
	//DiretInput
	ComPtr<IDirectInput8> directInput;

	//マウスデバイス
	ComPtr<IDirectInputDevice8> mouse;
	DIMOUSESTATE mouseData;
	DIMOUSESTATE preMouseData;
	//キーボードデバイス
	ComPtr<IDirectInputDevice8> keyboard;
	//全キーの状態
	BYTE key[256] = {};
	//前回の全キーの状態
	BYTE preKey[256] = {};
	//ゲームパッドデバイス
	ComPtr<IDirectInputDevice8> gamepad;
	DIJOYSTATE padData;
	DIJOYSTATE prePadData;

};