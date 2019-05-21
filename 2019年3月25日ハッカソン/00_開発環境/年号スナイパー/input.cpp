//=============================================================================
//
// 入力処理 [input.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "input.h"		// 入力

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECTINPUT8		CInput::m_pInput = NULL;		// オブジェクト

//=============================================================================
// コンストラクタ
//=============================================================================
CInput::CInput()
{
	// 初期値設定
	m_pDevice = NULL;		// デバイス
}

//=============================================================================
// デストラクタ
//=============================================================================
CInput::~CInput()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{// NULLの場合
		// DirectInputオブジェクトの生成
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInput::Uninit(void)
{
	// 入力デバイスの開放
	if (m_pDevice != NULL)
	{// NULL以外の場合
		m_pDevice->Unacquire();	// キーボードへのアクセス権を放棄
		m_pDevice->Release();	// 解放
		m_pDevice = NULL;		// NULLへ
	}

	// DirectInputオブジェクトの開放
	if (m_pInput != NULL)
	{//NULL以外の場合
		m_pInput->Release();	// 解放
		m_pInput = NULL;		// NULLへ
	}
}

//----------------------------------------------------------------------------------------------------

//=============================================================================
// コンストラクタ
//=============================================================================
CInputKeyboard::CInputKeyboard()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);		// 親クラス初期化処理

	if (m_pDevice == NULL)
	{// NULLの場合
		// 入力デバイス(キーボード)の生成
		if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
		{
			return E_FAIL;
		}

		if (m_pDevice != NULL)
		{
			// データフォーマットを設定
			if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
			{
				return E_FAIL;
			}

			// 協調モードを設定
			if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
			{
				return E_FAIL;
			}

			// キーボードへのアクセス権を獲得(入力制御開始)
			m_pDevice->Acquire();
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();		// 親クラス終了処理
}

//=============================================================================
// 更新処理
//=============================================================================
void CInputKeyboard::Update(void)
{
	if (m_pDevice != NULL)
	{// NULL以外の場合
		BYTE aKeyState[NUM_KEY_MAX];		// キーボード

		// 入力デバイスからデータを取得
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
		{
			for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
			{// カウント
				m_aKeyStateRelease[nCntKey] = (m_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyStatePress[nCntKey];
				m_aKeyStateTrigger[nCntKey] = (m_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
				m_aKeyStatePress[nCntKey] = aKeyState[nCntKey];
			}
		}
		else
		{
			m_pDevice->Acquire();		// キーボードへのアクセス権を獲得
		}
	}
}

//=============================================================================
// キーボードの入力情報(オール情報)を取得
//=============================================================================
bool CInputKeyboard::GetALL(int nType)
{
	switch (nType)
	{// タイプ番号
	case 0:
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{// カウント
			if (m_aKeyStatePress[nCntKey] == 0x80)
			{// プレス
				return true;
			}
		}

		break;
	case 1:
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{// カウント
			if (m_aKeyStateTrigger[nCntKey] == 0x80)
			{// トリガー
				return true;
			}
		}

		break;
	case 2:
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{// カウント
			if (m_aKeyStateRelease[nCntKey] == 0x80)
			{// リリース
				return true;
			}
		}

		break;
	}

	return false;
}

//=============================================================================
// キーボードの入力情報(プレス情報)を取得
//=============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyStatePress[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードの入力情報(トリガー情報)を取得
//=============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// キーボードの入力情報(リリース情報)を取得
//=============================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//----------------------------------------------------------------------------------------------------

//=============================================================================
// コンストラクタ
//=============================================================================
CInputDIPad::CInputDIPad()
{
	// 初期値設定
	m_LStickRot = 0.0f;		// Lスティック
	m_RStickRot = 0.0f;		// Rスティック
}

//=============================================================================
// デストラクタ
//=============================================================================
CInputDIPad::~CInputDIPad()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInputDIPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);		// 親クラス初期化処理

	if (m_pDevice == NULL)
	{// NULLの場合
		// 入力デバイス(DIパッド)の生成
		if (FAILED(m_pInput->CreateDevice(GUID_Joystick, &m_pDevice, NULL)))
		{
			//return E_FAIL;
		}

		if (m_pDevice != NULL)
		{
			// データフォーマットを設定
			if (FAILED(m_pDevice->SetDataFormat(&c_dfDIJoystick)))
			{
				return E_FAIL;
			}

			// 協調モードを設定
			if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
			{
				return E_FAIL;
			}

			DIPROPRANGE diprg;

			// 軸の値の範囲を設定
			diprg.diph.dwSize = sizeof(DIPROPRANGE);
			diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			diprg.diph.dwHow = DIPH_BYOFFSET;
			diprg.lMin = 0 - 1000;		// ジョイスティックの十字キーの最小値（上、左）
			diprg.lMax = 0 + 1000;		// ジョイスティックの十字キーの最大値（下、右）

			// 軸の設定
			// 左アナログスティック
			diprg.diph.dwObj = DIJOFS_X;
			m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
			diprg.diph.dwObj = DIJOFS_Y;
			m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
			diprg.diph.dwObj = DIJOFS_Z;
			m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

			// 右アナログスティック
			diprg.diph.dwObj = DIJOFS_RX;
			m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
			diprg.diph.dwObj = DIJOFS_RY;
			m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
			diprg.diph.dwObj = DIJOFS_RZ;
			m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

			// 十字キーの設定
			diprg.diph.dwObj = DIJOFS_POV(0);
			m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

			// DIパッドへのアクセス権を獲得(入力制御開始)
			m_pDevice->Acquire();
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInputDIPad::Uninit(void)
{
	CInput::Uninit();		// 親クラス終了処理
}

//=============================================================================
// 更新処理
//=============================================================================
void CInputDIPad::Update(void)
{
	if (m_pDevice != NULL)
	{// NULL以外の場合
		DIJOYSTATE dJoyState;		// ジョイパット
		BYTE bButton;				// ボタン

		m_pDevice->Poll();
		// 入力デバイスからデータを取得
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(dJoyState), &dJoyState)))
		{
			m_RStickRotX = (float)dJoyState.lZ;
			m_RStickRotY = (float)dJoyState.lRz;

			for (int nCntJoyPad = 0; nCntJoyPad < DIPADOTHER_MAX; nCntJoyPad++)
			{// カウント
				if (nCntJoyPad < NUM_BUTTON_MAX)
				{// ボタン
					bButton = dJoyState.rgbButtons[nCntJoyPad];
				}
				else
				{
					switch (nCntJoyPad)
					{
					case DIPADOTHER_AXES_UP:		// 十字キー上
						if (dJoyState.rgdwPOV[0] == 4500 || dJoyState.rgdwPOV[0] == 0 || dJoyState.rgdwPOV[0] == 31500)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_AXES_LEFT:		// 十字キー左
						if (dJoyState.rgdwPOV[0] == 4500 || dJoyState.rgdwPOV[0] == 9000 || dJoyState.rgdwPOV[0] == 13500)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_AXES_RIGHT:		// 十字キー右
						if (dJoyState.rgdwPOV[0] == 13500 || dJoyState.rgdwPOV[0] == 18000 || dJoyState.rgdwPOV[0] == 22500)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_AXES_DOWN:		// 十字キー下
						if (dJoyState.rgdwPOV[0] == 22500 || dJoyState.rgdwPOV[0] == 27000 || dJoyState.rgdwPOV[0] == 31500)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_STICK_L_UP:		// Lスティック上
						if (100 < dJoyState.lY)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_STICK_L_LEFT:	// Lスティック左
						if (dJoyState.lX < -100)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_STICK_L_RIGHT:	// Lスティック右
						if (100 < dJoyState.lX)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_STICK_L_DOWN:	// Lスティック下
						if (dJoyState.lY < -100)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_STICK_R_UP:		// Rスティック上
						if (100 < dJoyState.lRz)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_STICK_R_LEFT:	// Rスティック左
						if (dJoyState.lZ < -100)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_STICK_R_RIGHT:	// Rスティック右
						if (100 < dJoyState.lZ)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_STICK_R_DOWN:	// Rスティック下
						if (dJoyState.lRz < -100)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					}
				}

				// スティック向き
				m_LStickRot = atan2f((float)dJoyState.lX, (float)dJoyState.lY);
				m_RStickRot = atan2f((float)dJoyState.lZ, (float)dJoyState.lRz);

				// ジョイパッドの入力情報
				m_aJoyStateRelease[nCntJoyPad] = (m_aJoyStatePress[nCntJoyPad] ^ bButton) & m_aJoyStatePress[nCntJoyPad];
				m_aJoyStateTrigger[nCntJoyPad] = (m_aJoyStatePress[nCntJoyPad] ^ bButton) & bButton;
				m_aJoyStatePress[nCntJoyPad] = bButton;
			}
		}
		else
		{
			m_pDevice->Acquire();		// キーボードへのアクセス権を獲得
		}
	}
}

//=============================================================================
// DIパッドの入力情報(プレス情報)を取得
//=============================================================================
bool CInputDIPad::GetPress(int nButton)
{
	return (m_aJoyStatePress[nButton] & 0x80) ? true : false;
}

//=============================================================================
// DIパッドの入力情報(トリガー情報)を取得
//=============================================================================
bool CInputDIPad::GetTrigger(int nButton)
{
	return (m_aJoyStateTrigger[nButton] & 0x80) ? true : false;
}

//=============================================================================
// DIパッドの入力情報(リリース情報)を取得\
//=============================================================================
bool CInputDIPad::GetRelease(int nButton)
{
	return (m_aJoyStateRelease[nButton] & 0x80) ? true : false;
}

//=============================================================================
// DIパッドの入力情報(スティック情報)を取得	\
//=============================================================================
bool CInputDIPad::GetStick(int nLR)
{
	if (nLR == 0)
	{
		for (int nCntStick = DIPADOTHER_STICK_L_UP; nCntStick <= DIPADOTHER_STICK_L_DOWN; nCntStick++)
		{
			if (m_aJoyStatePress[nCntStick] == 0x80)
			{
				return true;
			}
		}
	}
	else
	{
		for (int nCntStick = DIPADOTHER_STICK_R_UP; nCntStick <= DIPADOTHER_STICK_R_DOWN; nCntStick++)
		{
			if (m_aJoyStatePress[nCntStick] == 0x80)
			{
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
// DIパッドの入力情報(スティック向き情報)を取得
//=============================================================================
float CInputDIPad::GetStickRot(int nLR)
{
	if (nLR == 0)
	{
		return m_LStickRot;
	}
	else
	{
		return m_RStickRot;
	}
}
//=============================================================================
// スティックの軸X
//=============================================================================
float CInputDIPad::GetDRightXAxis(void)
{
	return m_RStickRotX;
}

//=============================================================================
// スティックの軸Y
//=============================================================================
float CInputDIPad::GetDRightYAxis(void)
{
	return m_RStickRotY;
}
//=============================================================================
// Xパッドの軸入力情報(スティック向き情報)を取得
//=============================================================================
float  CInputDIPad::GetDRightAxis(void)
{
	float fAxis = atan2f(m_RStickRotX, m_RStickRotY);
	return fAxis;
}

//----------------------------------------------------------------------------------------------------
int CInputXPad::m_nInputNum = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CInputXPad::CInputXPad()
{
	// 初期値設定
	for (int nCntCont = 0; nCntCont < MAX_CONTROLLERS; nCntCont++)
	{
		m_LStickRot[nCntCont] = 0.0f;		// Lスティック
		m_RStickRot[nCntCont] = 0.0f;		// Rスティック
	}

	m_nInputNum = 0;						// 入力数
}

//=============================================================================
// デストラクタ
//=============================================================================
CInputXPad::~CInputXPad()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CInputXPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// コントローラーの初期化
	ZeroMemory(m_Controllers, sizeof(CONTROLER_STATE) * MAX_CONTROLLERS);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CInputXPad::Uninit(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void CInputXPad::Update(void)
{
	UpdateControllerState();	// 途中入力

	for (DWORD nCntCont = 0; nCntCont < MAX_CONTROLLERS; nCntCont++)
	{// コントローラーカウント
		if (m_Controllers[nCntCont].bConnected == true)
		{// 接続されていたら
			if (m_bDeadZoneOn == true)
			{// スティック
			 // Zero value if thumbsticks are within the dead zone
				if ((m_Controllers[nCntCont].state.Gamepad.sThumbLX < INPUT_DEADZONE &&
					m_Controllers[nCntCont].state.Gamepad.sThumbLX > -INPUT_DEADZONE) &&
					(m_Controllers[nCntCont].state.Gamepad.sThumbLY < INPUT_DEADZONE &&
						m_Controllers[nCntCont].state.Gamepad.sThumbLY > -INPUT_DEADZONE))
				{
					m_Controllers[nCntCont].state.Gamepad.sThumbLX = 0;
					m_Controllers[nCntCont].state.Gamepad.sThumbLY = 0;
				}
				//m_Controllers[nCntCont].state.Gamepad.wButtons
				if ((m_Controllers[nCntCont].state.Gamepad.sThumbRX < INPUT_DEADZONE &&
					m_Controllers[nCntCont].state.Gamepad.sThumbRX > -INPUT_DEADZONE) &&
					(m_Controllers[nCntCont].state.Gamepad.sThumbRY < INPUT_DEADZONE &&
						m_Controllers[nCntCont].state.Gamepad.sThumbRY > -INPUT_DEADZONE))
				{
					m_Controllers[nCntCont].state.Gamepad.sThumbRX = 0;
					m_Controllers[nCntCont].state.Gamepad.sThumbRY = 0;
				}
			}

			WORD wButtons = 0;		// XINPUTコントローラーの入力情報

			for (int nCount = 0; nCount < XPADOTHER_MAX; nCount++)
			{// 入力情報カウント
				switch (nCount)
				{
				case 0:		// ボタン
					wButtons = m_Controllers[nCntCont].state.Gamepad.wButtons;
					break;
				case 1:		// 左トリガー
					wButtons = m_Controllers[nCntCont].state.Gamepad.bLeftTrigger;
					break;
				case 2:		// 右トリガー
					wButtons = m_Controllers[nCntCont].state.Gamepad.bRightTrigger;
					break;
				case 3:		// 左スティック上
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLY > XINPUT_STICK_MAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 4:		// 左スティック左
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLX < XINPUT_STICK_MIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 5:		// 左スティック右
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLX > XINPUT_STICK_MAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 6:		// 左スティック下
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLY < XINPUT_STICK_MIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 7:		// 右スティック上
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRY > XINPUT_STICK_MAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 8:		// 右スティック左
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRX < XINPUT_STICK_MIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 9:		// 右スティック右
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRX > XINPUT_STICK_MAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 10:		// 右スティック下
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRY < XINPUT_STICK_MIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				}

				// スティック向き
				m_LStickRot[nCntCont] = atan2f((float)m_Controllers[nCntCont].state.Gamepad.sThumbLX, (float)m_Controllers[nCntCont].state.Gamepad.sThumbLY);
				m_RStickRot[nCntCont] = atan2f((float)m_Controllers[nCntCont].state.Gamepad.sThumbRX, (float)m_Controllers[nCntCont].state.Gamepad.sThumbRY);

				// ジョイパッドの入力情報
				m_aJoyStateTrigger[nCntCont][nCount] = (m_aJoyStatePress[nCntCont][nCount] ^ wButtons) & wButtons;
				m_aJoyStateRelease[nCntCont][nCount] = (m_aJoyStatePress[nCntCont][nCount] ^ wButtons) & m_aJoyStatePress[nCntCont][nCount];
				m_aJoyStatePress[nCntCont][nCount] = wButtons;
			}
		}
	}
}

//=============================================================================
// Xパッドの入力情報(入力確認)を取得
//=============================================================================
HRESULT CInputXPad::UpdateControllerState(void)
{
	DWORD dwResult;
	m_nInputNum = 0;
	for (DWORD nCntCont = 0; nCntCont < MAX_CONTROLLERS; nCntCont++)
	{
		// Simply get the state of the controller from XInput.
		dwResult = XInputGetState(nCntCont, &m_Controllers[nCntCont].state);

		if (dwResult == ERROR_SUCCESS)
		{
			m_Controllers[nCntCont].bConnected = true;
			m_nInputNum++;
		}
		else
		{
			m_Controllers[nCntCont].bConnected = false;
		}
	}

	return S_OK;
}
//=============================================================================
// Xパッドの入力情報(入力数情報)を取得
//=============================================================================
int CInputXPad::GetInputNum(void)
{
	return m_nInputNum;
}

//=============================================================================
// Xパッドの入力情報(オール情報)を取得
//=============================================================================
bool CInputXPad::GetALL(int nType, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
	{
		switch (nType)
		{
		case 0:
			for (int nCount = 0; nCount < XPADOTHER_MAX; nCount++)
			{// 入力情報カウント
				if (nCount == XPADOTHER_TRIGGER_LEFT || nCount == XPADOTHER_TRIGGER_RIGHT ||
					nCount == XPADOTHER_STICK_L_UP || nCount == XPADOTHER_STICK_L_LEFT || nCount == XPADOTHER_STICK_L_RIGHT || nCount == XPADOTHER_STICK_L_DOWN ||
					nCount == XPADOTHER_STICK_R_UP || nCount == XPADOTHER_STICK_R_LEFT || nCount == XPADOTHER_STICK_R_RIGHT || nCount == XPADOTHER_STICK_R_DOWN)
				{
					if (m_aJoyStatePress[nIdxPad][nCount] == 0x80)
					{
						return true;
					}
				}
				else
				{
					if (m_aJoyStatePress[nIdxPad][nCount] != 0)
					{
						return true;
					}
				}
			}

			break;
		case 1:
			for (int nCount = 0; nCount < XPADOTHER_MAX; nCount++)
			{// 入力情報カウント
				if (nCount == XPADOTHER_TRIGGER_LEFT || nCount == XPADOTHER_TRIGGER_RIGHT ||
					nCount == XPADOTHER_STICK_L_UP || nCount == XPADOTHER_STICK_L_LEFT || nCount == XPADOTHER_STICK_L_RIGHT || nCount == XPADOTHER_STICK_L_DOWN ||
					nCount == XPADOTHER_STICK_R_UP || nCount == XPADOTHER_STICK_R_LEFT || nCount == XPADOTHER_STICK_R_RIGHT || nCount == XPADOTHER_STICK_R_DOWN)
				{
					if (m_aJoyStateTrigger[nIdxPad][nCount] == 0x80)
					{
						return true;
					}
				}
				else
				{
					if (m_aJoyStateTrigger[nIdxPad][nCount] != 0)
					{
						return true;
					}
				}
			}

			break;
		default:
			for (int nCount = 0; nCount < XPADOTHER_MAX; nCount++)
			{// 入力情報カウント
				if (nCount == XPADOTHER_TRIGGER_LEFT || nCount == XPADOTHER_TRIGGER_RIGHT ||
					nCount == XPADOTHER_STICK_L_UP || nCount == XPADOTHER_STICK_L_LEFT || nCount == XPADOTHER_STICK_L_RIGHT || nCount == XPADOTHER_STICK_L_DOWN ||
					nCount == XPADOTHER_STICK_R_UP || nCount == XPADOTHER_STICK_R_LEFT || nCount == XPADOTHER_STICK_R_RIGHT || nCount == XPADOTHER_STICK_R_DOWN)
				{
					if (m_aJoyStateRelease[nIdxPad][nCount] == 0x80)
					{
						return true;
					}
				}
				else
				{
					if (m_aJoyStateRelease[nIdxPad][nCount] != 0)
					{
						return true;
					}
				}
			}

			break;
		}
	}

	return false;
}

//=============================================================================
// Xパッドの入力情報(プレス情報)を取得
//=============================================================================
bool CInputXPad::GetPress(int nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStatePress[nIdxPad][0] & nButton) ? true : false;
	else
		return false;
}
bool CInputXPad::GetPress(XPADOTHER nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStatePress[nIdxPad][nButton] & 0x80) ? true : false;
	else
		return false;
}

//=============================================================================
// Xパッドの入力情報(トリガー情報)を取得
//=============================================================================
bool CInputXPad::GetTrigger(int nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateTrigger[nIdxPad][0] & nButton) ? true : false;
	else
		return false;
}
bool CInputXPad::GetTrigger(XPADOTHER nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateTrigger[nIdxPad][nButton] & 0x80) ? true : false;
	else
		return false;
}

//=============================================================================
// Xパッドの入力情報(リリース情報)を取得
//=============================================================================
bool CInputXPad::GetRelease(int nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateRelease[nIdxPad][0] & nButton) ? true : false;
	else
		return false;
}
bool CInputXPad::GetRelease(XPADOTHER nButton, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
		return (m_aJoyStateRelease[nIdxPad][nButton] & 0x80) ? true : false;
	else
		return false;
}

//=============================================================================
// Xパッドの入力情報(スティック情報)を取得
//=============================================================================
bool CInputXPad::GetStick(int nLR, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
	{
		if (nLR == 0)
		{
			for (int nCntStick = XPADOTHER_STICK_L_UP; nCntStick <= XPADOTHER_STICK_L_DOWN; nCntStick++)
			{
				if (m_aJoyStatePress[nIdxPad][nCntStick] == 0x80)
				{
					return true;
				}
			}
		}
		else
		{
			for (int nCntStick = XPADOTHER_STICK_R_UP; nCntStick <= XPADOTHER_STICK_R_DOWN; nCntStick++)
			{
				if (m_aJoyStatePress[nIdxPad][nCntStick] == 0x80)
				{
					return true;
				}
			}
		}
	}

	return false;
}

//=============================================================================
// Xパッドの入力情報(スティック向き情報)を取得
//=============================================================================
float CInputXPad::GetStickRot(int nLR, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
	{
		if (nLR == 0)
		{
			return m_LStickRot[nIdxPad];
		}
		else
		{
			return m_RStickRot[nIdxPad];
		}
	}

	return false;
}
//=============================================================================
// スティックの軸X
//=============================================================================
float CInputXPad::GetRightXAxis(void)
{
	return m_RStickRotX;
}

//=============================================================================
// スティックの軸Y
//=============================================================================
float CInputXPad::GetRightYAxis(void)
{
	return m_RStickRotY;
}
//=============================================================================
// Xパッドの軸入力情報(スティック向き情報)を取得
//=============================================================================
float  CInputXPad::GetRightAxis(void)
{
	float fAxis = atan2f(m_RStickRotY, m_RStickRotX);
	return fAxis;
}