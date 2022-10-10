#pragma once

#include <WinApp.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800          // DirectInputのバージョン指定
#include <dinput.h>

#define SticLeft	0
#define SticRight	1
#define SticUp		2
#define SticDown	3

#define JoyAButton	0
#define JoyBButton	1
#define JoyXButton	2
#define JoyYButton	3
#define JoyL1Button	4
#define JoyR1Button	5
#define JoyBackButton	6
#define JoyStartButton	7
#define JoyL3Button	8

#define JoyUpButton	0
#define JoyRightButton	1
#define JoyDownButton	2
#define JoyLeftButton	3


/// <summary>
/// 入力
/// </summary>
class Input
{
public:
	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};

private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // 静的メンバ変数
	static Input* GetInstance();

	static BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);

public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(WinApp* win);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// キーの左ボタン押下をチェック
	/// </summary>
	/// <returns>押されているか</returns>
	bool PushMouseLeft();

	/// <summary>
	/// キーの中ボタン押下をチェック
	/// </summary>
	/// <returns>押されているか</returns>
	bool PushMouseMiddle();

	/// <summary>
	/// キーの左ボタントリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool TriggerMouseLeft();

	/// <summary>
	/// キーの中ボタントリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool TriggerMouseMiddle();

	/// <summary>
	/// マウス移動量を取得
	/// </summary>
	/// <returns>マウス移動量</returns>
	MouseMove GetMouseMove();

	/// <summary>
	/// ゲームパッドのデータ取得
	/// </summary>
	/// <returns>ゲームパッドのデータ</returns>
	DIJOYSTATE2 GetJoyState();

	/// <summary>
	/// ゲームパッドの左側スティックデータ取得
	/// </summary>
	/// <param name="sticNum">スティックナンバー(例 SticLeft)</param>
	/// <returns>成否</returns>
	bool GetLeftStic(unsigned int sticNumber);

	/// <summary>
	/// ゲームパッドの右側スティックデータ取得
	/// </summary>
	/// <param name="sticNumber">スティックナンバー(例 SticLeft)</param>
	/// <returns>成否</returns>
	bool GetRightStic(unsigned int sticNumber);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="sticNum"></param>
	/// <returns></returns>
	bool GetTriggerLeftStic(unsigned int sticNumber);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="sticNum"></param>
	/// <returns></returns>
	bool GetTriggerRightStic(unsigned int sticNumber);


	bool GetJoyButton(unsigned int buttonNumber);


	bool GetTriggerJoyButton(unsigned int buttonNumber);


	bool GetJoyCrossButton(unsigned int buttonNumber);
	bool GetTriggerJoyCrossButton(unsigned int buttonNumber);

	/// <summary>
	/// バッドが刺さっているか
	/// </summary>
	/// <returns></returns>
	bool GetJoyPadSet() 
	{ if (devJoypad != nullptr) { return true; } return false; }

private: // メンバ関数
	Input() = default;
	Input(const Input&) = delete;
	~Input() = default;

	/// <summary>
	/// ゲームパッドパッドの初期化
	/// </summary>
	/// <returns>成否</returns>
	bool InitializeJoyPad(HWND hwnd);


	Input& operator=(const Input&) = delete;

private: // メンバ変数
	WinApp* win = nullptr;
	ComPtr<IDirectInput8> dinput;
	ComPtr<IDirectInputDevice8> devKeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};
	ComPtr<IDirectInputDevice8> devJoypad;
	DIJOYSTATE2 joyState;
	DIJOYSTATE2 joyStatePre;
	bool isGamepadPush[32];
};

