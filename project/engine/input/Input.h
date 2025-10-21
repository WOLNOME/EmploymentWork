#pragma once
#include <wrl.h>
#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>
#include "Vector2.h"

/// <summary>
/// マウスのコマンド
/// </summary>
enum MouseButton {
	LeftButton,    // 左ボタン
	RightButton,   // 右ボタン
	MiddleButton   // 中央ボタン
};

/// <summary>
/// ゲームパッドのコマンド
/// </summary>
enum GamepadButton {
	ButtonA,		// Aボタン
	ButtonB,		// Bボタン
	ButtonX,		// Xボタン
	ButtonY,		// Yボタン
	LeftShoulder,	// LBボタン
	RightShoulder,	// RBボタン
	Back,			// BACKボタン
	Start,			// STARTボタン
	LeftThumb,		// 左スティック押し込み
	RightThumb,		// 右スティック押し込み
	DPadUp,			// 十字キー 上
	DPadDown,		// 十字キー 下
	DPadLeft,		// 十字キー 左
	DPadRight		// 十字キー 右
};

/// <summary>
/// デバイスから入力される操作信号を判定するクラス
/// シングルトンパターンで実装
/// </summary>
class Input {
private://コンストラクタ等の隠蔽
	static Input* instance;

	Input() = default;//コンストラクタ隠蔽
	~Input() = default;//デストラクタ隠蔽
	Input(Input&) = delete;//コピーコンストラクタ封印
	Input& operator=(Input&) = delete;//コピー代入演算子封印
public:
	/// ============================== ///
	///		メンバ関数
	/// ============================== ///

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns></returns>
	static Input* GetInstance();

	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;//エイリアステンプレート

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// ============================== ///
	///		コマンド操作関数
	/// ============================== ///

	/// <summary>
	/// 指定したマウスボタンが現在押されているかを判定する
	/// </summary>
	/// <param name="button">判定対象のマウスボタン</param>
	/// <returns>押されていれば true、押されていなければ false</returns>
	bool PushMouseButton(MouseButton button);
	/// <summary>
	/// 指定したマウスボタンが「このフレームで押された瞬間」であるかを判定する
	/// </summary>
	/// <param name="button">判定対象のマウスボタン</param>
	/// <returns>押された瞬間であれば true、そうでなければ false</returns>
	bool TriggerMouseButton(MouseButton button);
	/// <summary>
	/// 指定したキーが現在押されているかを判定する
	/// </summary>
	/// <param name="keyNumber">判定対象のキー番号（例：DIK_SPACE など）</param>
	/// <returns>押されていれば true、押されていなければ false</returns>
	bool PushKey(BYTE keyNumber);
	/// <summary>
	/// 指定したキーが「このフレームで押された瞬間」であるかを判定する
	/// </summary>
	/// <param name="keyNumber">判定対象のキー番号（例：DIK_RETURN など）</param>
	/// <returns>押された瞬間であれば true、そうでなければ false</returns>
	bool TriggerKey(BYTE keyNumber);
	/// <summary>
	/// 指定したゲームパッドボタンが現在押されているかを判定する
	/// </summary>
	/// <param name="button">判定対象のゲームパッドボタン</param>
	/// <returns>押されていれば true、押されていなければ false</returns>
	bool PushPadButton(GamepadButton button);
	/// <summary>
	/// 指定したゲームパッドボタンが「このフレームで押された瞬間」であるかを判定する
	/// </summary>
	/// <param name="button">判定対象のゲームパッドボタン</param>
	/// <returns>押された瞬間であれば true、そうでなければ false</returns>
	bool TriggerPadButton(GamepadButton button);
	/// <summary>
	/// 現在のマウス座標を取得する
	/// </summary>
	/// <returns>マウス座標（ウィンドウ座標系）</returns>
	Vector2 GetMousePosition();
	/// <summary>
	/// マウスホイールの回転量を取得する
	/// </summary>
	/// <returns>
	/// スクロール量（上方向が正の値、下方向が負の値）
	/// </returns>
	float GetMouseScrollCount();
	/// <summary>
	/// ゲームパッドの左スティックの入力方向を取得する
	/// </summary>
	/// <returns>左スティックの入力方向（正規化済みベクトル）</returns>
	Vector2 GetLeftStickDir();
	/// <summary>
	/// ゲームパッドの右スティックの入力方向を取得する
	/// </summary>
	/// <returns>右スティックの入力方向（正規化済みベクトル）</returns>
	Vector2 GetRightStickDir();
	/// <summary>
	/// マウスカーソルの表示／非表示を設定する
	/// </summary>
	/// <param name="_isDisplay">true ならカーソルを表示、false なら非表示</param>
	void SetIsMouseDisplay(bool _isDisplay);
	/// <summary>
	/// マウスカーソルの固定状態を設定する（画面中央に固定するなど）
	/// </summary>
	/// <param name="_isMiddle">true ならカーソルを固定、false なら自由に動かす</param>
	void SetIsMouseFixed(bool _isMiddle);

private:
	/// ============================== ///
	///		非公開メンバ関数
	/// ============================== ///

	/// <summary>
	/// DirectInputの初期化を行う
	/// </summary>
	void InitDirectInput();
	/// <summary>
	/// マウスデバイスを生成する
	/// </summary>
	void GenerateMouse();
	/// <summary>
	/// キーボードデバイスを生成する
	/// </summary>
	void GenerateKeyboard();
	/// <summary>
	/// ゲームパッドデバイスを生成する
	/// </summary>
	void GenerateGamepad();

	/// ============================== ///
	///		メンバ変数
	/// ============================== ///

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