#include "Input.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")

Input* Input::GetInstance()
{
	static Input instance;

	return &instance;
}

BOOL Input::DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}

bool Input::Initialize(WinApp* win)
{
	HRESULT result = S_FALSE;

	assert(win);
	assert(!dinput);

	this->win = win;

	// DirectInputオブジェクトの生成	
	result = DirectInput8Create(win->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// キーボードデバイスの生成	
	result = dinput->CreateDevice(GUID_SysKeyboard, &devKeyboard, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// マウスデバイスの生成	
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// デバイスの列挙
	result = dinput->EnumDevices(DI8DEVTYPE_JOYSTICK, DeviceFindCallBack, NULL, DIEDFL_ATTACHEDONLY);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// ゲームパッドデバイスの生成
	result = dinput->CreateDevice(GUID_Joystick, &devJoypad, NULL);

	// 入力データ形式のセット
	result = devKeyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 排他制御レベルのセット
	result = devKeyboard->SetCooperativeLevel(win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 入力データ形式のセット
	result = devMouse->SetDataFormat(&c_dfDIMouse2); // 標準形式
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 排他制御レベルのセット
	result = devMouse->SetCooperativeLevel(win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// デバイスのフォーマットの設定
	if (devJoypad != nullptr)
	{
		result = InitializeJoyPad(win->GetHwnd());
		if (FAILED(result)) {
			assert(0);
			return result;
		}
	}

	return true;
}

void Input::Update()
{
	HRESULT result;

	{// キーボード
		result = devKeyboard->Acquire();	// キーボード動作開始

		// 前回のキー入力を保存
		memcpy(keyPre, key, sizeof(key));

		// キーの入力
		result = devKeyboard->GetDeviceState(sizeof(key), key);
	}

	{// マウス
		result = devMouse->Acquire();	// マウス動作開始

		// 前回の入力を保存
		mouseStatePre = mouseState;

		// マウスの入力
		result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	}

	// ゲームパッドが確認されていなければ
	if (devJoypad == nullptr)
	{
		// ゲームパッドデバイスの生成
		result = dinput->CreateDevice(GUID_Joystick, &devJoypad, NULL);

		// デバイスのフォーマットの設定
		if (devJoypad != nullptr)
		{
			result = InitializeJoyPad(win->GetHwnd());
			if (FAILED(result)) {
				assert(0);
				return;
			}
		}
	}


	if (devJoypad != nullptr)
	{
		// ゲームパッド
		result = devJoypad->Acquire();		// ゲームパッド動作開始

		// 前回の入力を保存
		joyStatePre = joyState;

		// ゲームパッドの入力
		result = devJoypad->GetDeviceState(sizeof(joyState), &joyState);

		devJoypad->Poll();
	}
}

bool Input::PushKey(BYTE keyNumber)
{
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);

	// 0でなければ押している
	if (key[keyNumber]) {
		return true;
	}

	// 押していない
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);

	// 前回が0で、今回が0でなければトリガー
	if (!keyPre[keyNumber] && key[keyNumber]) {
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::PushMouseLeft()
{
	// 0でなければ押している
	if (mouseState.rgbButtons[0]) {
		return true;
	}

	// 押していない
	return false;
}

bool Input::PushMouseMiddle()
{
	// 0でなければ押している
	if (mouseState.rgbButtons[2]) {
		return true;
	}

	// 押していない
	return false;
}

bool Input::TriggerMouseLeft()
{
	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre.rgbButtons[0] && mouseState.rgbButtons[0]) {
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::TriggerMouseMiddle()
{
	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre.rgbButtons[2] && mouseState.rgbButtons[2]) {
		return true;
	}

	// トリガーでない
	return false;
}

Input::MouseMove Input::GetMouseMove()
{
	MouseMove tmp;
	tmp.lX = mouseState.lX;
	tmp.lY = mouseState.lY;
	tmp.lZ = mouseState.lZ;
	return tmp;
}

DIJOYSTATE2 Input::GetJoyState()
{
	if (devJoypad != nullptr) {
		return joyState;
	}

	return DIJOYSTATE2();
}

bool Input::GetLeftStic(unsigned int sticNumber)
{
	// 異常な引数を検出
	assert(0 <= sticNumber && sticNumber <= 3);

	if (devJoypad != nullptr)
	{
		// スティックナンバーがLeftの場合
		if (sticNumber == SticLeft)
		{
			if (joyState.lX < -500) { return true; }
		}
		// スティックナンバーがRightの場合
		if (sticNumber == SticRight)
		{
			if (joyState.lX > +500) { return true; }
		}
		// スティックナンバーがUpの場合
		if (sticNumber == SticUp)
		{
			if (joyState.lY < -500) { return true; }
		}
		// スティックナンバーがDownの場合
		if (sticNumber == SticDown)
		{
			if (joyState.lY > +500) { return true; }
		}
	}
	return false;
}

bool Input::GetRightStic(unsigned int sticNumber)
{
	// 異常な引数を検出
	assert(0 <= sticNumber && sticNumber <= 3);

	if (devJoypad != nullptr)
	{
		// スティックナンバーがLeftの場合
		if (sticNumber == SticLeft)
		{
			if (joyState.lRx < -500) { return true; }
		}
		// スティックナンバーがRightの場合
		if (sticNumber == SticRight)
		{
			if (joyState.lRx > +500) { return true; }
		}
		// スティックナンバーがUpの場合
		if (sticNumber == SticUp)
		{
			if (joyState.lRy < -500) { return true; }
		}
		// スティックナンバーがDownの場合
		if (sticNumber == SticDown)
		{
			if (joyState.lRy > +500) { return true; }
		}
	}

	return false;
}

bool Input::GetTriggerLeftStic(unsigned int sticNumber)
{
	// 異常な引数を検出
	assert(0 <= sticNumber && sticNumber <= 3);

	if (devJoypad != nullptr)
	{
		// スティックナンバーがLeftの場合
		if (sticNumber == SticLeft)
		{
			if (!(joyStatePre.lX < -500) && joyState.lX < -500) { return true; }
		}
		// スティックナンバーがRightの場合
		if (sticNumber == SticRight)
		{
			if (!(joyStatePre.lX > +500) && joyState.lX > +500) { return true; }
		}
		// スティックナンバーがUpの場合
		if (sticNumber == SticUp)
		{
			if (!(joyStatePre.lY < -500) && joyState.lY < -500) { return true; }
		}
		// スティックナンバーがDownの場合
		if (sticNumber == SticDown)
		{
			if (!(joyStatePre.lY > +500) && joyState.lY > +500) { return true; }
		}
	}
	return false;
}

bool Input::GetTriggerRightStic(unsigned int sticNumber)
{
	// 異常な引数を検出
	assert(0 <= sticNumber && sticNumber <= 3);

	if (devJoypad != nullptr)
	{
		// スティックナンバーがLeftの場合
		if (sticNumber == SticLeft)
		{
			if (!(joyStatePre.lRx < -500) && joyState.lRx < -500) { return true; }
		}
		// スティックナンバーがRightの場合
		if (sticNumber == SticRight)
		{
			if (!(joyStatePre.lRx > +500) && joyState.lRx > +500) { return true; }
		}
		// スティックナンバーがUpの場合
		if (sticNumber == SticUp)
		{
			if (!(joyStatePre.lRy < -500) && joyState.lRy < -500) { return true; }
		}
		// スティックナンバーがDownの場合
		if (sticNumber == SticDown)
		{
			if (!(joyStatePre.lRy > +500) && joyState.lRy > +500) { return true; }
		}
	}

	return false;
}

bool Input::GetJoyButton(unsigned int buttonNumber)
{
	// 異常な引数を検出
	assert(0 <= buttonNumber && buttonNumber <= 9);

	if (joyState.rgbButtons[buttonNumber] != 0) { return true; }

	return false;
}

bool Input::GetTriggerJoyButton(unsigned int buttonNumber)
{
	// 異常な引数を検出
	assert(0 <= buttonNumber && buttonNumber <= 9);

	if (joyStatePre.rgbButtons[buttonNumber] == 0 && joyState.rgbButtons[buttonNumber] != 0) { return true; }

	return false;
}

bool Input::GetJoyCrossButton(unsigned int buttonNumber)
{
	// 異常な引数を検出
	assert(0 <= buttonNumber && buttonNumber <= 3);

	if (buttonNumber == JoyUpButton)
	{
		if (joyState.rgdwPOV[0] == 0) { return true; }
	}
	if (buttonNumber == JoyRightButton)
	{
		if (joyState.rgdwPOV[0] == 9000) { return true; }
	}
	if (buttonNumber == JoyDownButton)
	{
		if (joyState.rgdwPOV[0] == 18000) { return true; }
	}
	if (buttonNumber == JoyLeftButton)
	{
		if (joyState.rgdwPOV[0] == 27000) { return true; }
	}
	return false;
}

bool Input::GetTriggerJoyCrossButton(unsigned int buttonNumber)
{
	// 異常な引数を検出
	assert(0 <= buttonNumber && buttonNumber <= 3);

	if (buttonNumber == JoyUpButton)
	{
		if (!(joyStatePre.rgdwPOV[0] == 0) && joyState.rgdwPOV[0] == 0) { return true; }
	}
	if (buttonNumber == JoyRightButton)
	{
		if (!(joyStatePre.rgdwPOV[0] == 9000) && joyState.rgdwPOV[0] == 9000) { return true; }
	}
	if (buttonNumber == JoyDownButton)
	{
		if (!(joyStatePre.rgdwPOV[0] == 18000) && joyState.rgdwPOV[0] == 18000) { return true; }
	}
	if (buttonNumber == JoyLeftButton)
	{
		if (!(joyStatePre.rgdwPOV[0] == 27000) && joyState.rgdwPOV[0] == 27000) { return true; }
	}
	return false;
}

bool Input::InitializeJoyPad(HWND hwnd)
{
	HRESULT result = S_FALSE;

	// 入力データ形式のセット
	result = devJoypad->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;	// 絶対値モードの指定(DIPROPAXISMODE_RELにしたら相対値

	result = devJoypad->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// X軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	result = devJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}
	diprg.diph.dwObj = DIJOFS_RX;
	result = devJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	result = devJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}
	diprg.diph.dwObj = DIJOFS_RY;
	result = devJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
	if (FAILED(result)) {
		assert(0);
		return result;
	}


	// 排他制御レベルのセット
	result = devJoypad->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) {
		assert(0);
		return result;
	}


	return result;
}