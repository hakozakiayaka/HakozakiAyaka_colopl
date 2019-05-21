//=============================================================================
//
// ���͏��� [input.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "input.h"		// ����

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECTINPUT8		CInput::m_pInput = NULL;		// �I�u�W�F�N�g

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInput::CInput()
{
	// �����l�ݒ�
	m_pDevice = NULL;		// �f�o�C�X
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInput::~CInput()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{// NULL�̏ꍇ
		// DirectInput�I�u�W�F�N�g�̐���
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInput::Uninit(void)
{
	// ���̓f�o�C�X�̊J��
	if (m_pDevice != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pDevice->Unacquire();	// �L�[�{�[�h�ւ̃A�N�Z�X�������
		m_pDevice->Release();	// ���
		m_pDevice = NULL;		// NULL��
	}

	// DirectInput�I�u�W�F�N�g�̊J��
	if (m_pInput != NULL)
	{//NULL�ȊO�̏ꍇ
		m_pInput->Release();	// ���
		m_pInput = NULL;		// NULL��
	}
}

//----------------------------------------------------------------------------------------------------

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputKeyboard::CInputKeyboard()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);		// �e�N���X����������

	if (m_pDevice == NULL)
	{// NULL�̏ꍇ
		// ���̓f�o�C�X(�L�[�{�[�h)�̐���
		if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
		{
			return E_FAIL;
		}

		if (m_pDevice != NULL)
		{
			// �f�[�^�t�H�[�}�b�g��ݒ�
			if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
			{
				return E_FAIL;
			}

			// �������[�h��ݒ�
			if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
			{
				return E_FAIL;
			}

			// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
			m_pDevice->Acquire();
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();		// �e�N���X�I������
}

//=============================================================================
// �X�V����
//=============================================================================
void CInputKeyboard::Update(void)
{
	if (m_pDevice != NULL)
	{// NULL�ȊO�̏ꍇ
		BYTE aKeyState[NUM_KEY_MAX];		// �L�[�{�[�h

		// ���̓f�o�C�X����f�[�^���擾
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
		{
			for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
			{// �J�E���g
				m_aKeyStateRelease[nCntKey] = (m_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyStatePress[nCntKey];
				m_aKeyStateTrigger[nCntKey] = (m_aKeyStatePress[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
				m_aKeyStatePress[nCntKey] = aKeyState[nCntKey];
			}
		}
		else
		{
			m_pDevice->Acquire();		// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
		}
	}
}

//=============================================================================
// �L�[�{�[�h�̓��͏��(�I�[�����)���擾
//=============================================================================
bool CInputKeyboard::GetALL(int nType)
{
	switch (nType)
	{// �^�C�v�ԍ�
	case 0:
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{// �J�E���g
			if (m_aKeyStatePress[nCntKey] == 0x80)
			{// �v���X
				return true;
			}
		}

		break;
	case 1:
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{// �J�E���g
			if (m_aKeyStateTrigger[nCntKey] == 0x80)
			{// �g���K�[
				return true;
			}
		}

		break;
	case 2:
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{// �J�E���g
			if (m_aKeyStateRelease[nCntKey] == 0x80)
			{// �����[�X
				return true;
			}
		}

		break;
	}

	return false;
}

//=============================================================================
// �L�[�{�[�h�̓��͏��(�v���X���)���擾
//=============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyStatePress[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̓��͏��(�g���K�[���)���擾
//=============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �L�[�{�[�h�̓��͏��(�����[�X���)���擾
//=============================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//----------------------------------------------------------------------------------------------------

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputDIPad::CInputDIPad()
{
	// �����l�ݒ�
	m_LStickRot = 0.0f;		// L�X�e�B�b�N
	m_RStickRot = 0.0f;		// R�X�e�B�b�N
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInputDIPad::~CInputDIPad()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInputDIPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);		// �e�N���X����������

	if (m_pDevice == NULL)
	{// NULL�̏ꍇ
		// ���̓f�o�C�X(DI�p�b�h)�̐���
		if (FAILED(m_pInput->CreateDevice(GUID_Joystick, &m_pDevice, NULL)))
		{
			//return E_FAIL;
		}

		if (m_pDevice != NULL)
		{
			// �f�[�^�t�H�[�}�b�g��ݒ�
			if (FAILED(m_pDevice->SetDataFormat(&c_dfDIJoystick)))
			{
				return E_FAIL;
			}

			// �������[�h��ݒ�
			if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
			{
				return E_FAIL;
			}

			DIPROPRANGE diprg;

			// ���̒l�͈̔͂�ݒ�
			diprg.diph.dwSize = sizeof(DIPROPRANGE);
			diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			diprg.diph.dwHow = DIPH_BYOFFSET;
			diprg.lMin = 0 - 1000;		// �W���C�X�e�B�b�N�̏\���L�[�̍ŏ��l�i��A���j
			diprg.lMax = 0 + 1000;		// �W���C�X�e�B�b�N�̏\���L�[�̍ő�l�i���A�E�j

			// ���̐ݒ�
			// ���A�i���O�X�e�B�b�N
			diprg.diph.dwObj = DIJOFS_X;
			m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
			diprg.diph.dwObj = DIJOFS_Y;
			m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
			diprg.diph.dwObj = DIJOFS_Z;
			m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

			// �E�A�i���O�X�e�B�b�N
			diprg.diph.dwObj = DIJOFS_RX;
			m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
			diprg.diph.dwObj = DIJOFS_RY;
			m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
			diprg.diph.dwObj = DIJOFS_RZ;
			m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

			// �\���L�[�̐ݒ�
			diprg.diph.dwObj = DIJOFS_POV(0);
			m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

			// DI�p�b�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
			m_pDevice->Acquire();
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInputDIPad::Uninit(void)
{
	CInput::Uninit();		// �e�N���X�I������
}

//=============================================================================
// �X�V����
//=============================================================================
void CInputDIPad::Update(void)
{
	if (m_pDevice != NULL)
	{// NULL�ȊO�̏ꍇ
		DIJOYSTATE dJoyState;		// �W���C�p�b�g
		BYTE bButton;				// �{�^��

		m_pDevice->Poll();
		// ���̓f�o�C�X����f�[�^���擾
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(dJoyState), &dJoyState)))
		{
			m_RStickRotX = (float)dJoyState.lZ;
			m_RStickRotY = (float)dJoyState.lRz;

			for (int nCntJoyPad = 0; nCntJoyPad < DIPADOTHER_MAX; nCntJoyPad++)
			{// �J�E���g
				if (nCntJoyPad < NUM_BUTTON_MAX)
				{// �{�^��
					bButton = dJoyState.rgbButtons[nCntJoyPad];
				}
				else
				{
					switch (nCntJoyPad)
					{
					case DIPADOTHER_AXES_UP:		// �\���L�[��
						if (dJoyState.rgdwPOV[0] == 4500 || dJoyState.rgdwPOV[0] == 0 || dJoyState.rgdwPOV[0] == 31500)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_AXES_LEFT:		// �\���L�[��
						if (dJoyState.rgdwPOV[0] == 4500 || dJoyState.rgdwPOV[0] == 9000 || dJoyState.rgdwPOV[0] == 13500)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_AXES_RIGHT:		// �\���L�[�E
						if (dJoyState.rgdwPOV[0] == 13500 || dJoyState.rgdwPOV[0] == 18000 || dJoyState.rgdwPOV[0] == 22500)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_AXES_DOWN:		// �\���L�[��
						if (dJoyState.rgdwPOV[0] == 22500 || dJoyState.rgdwPOV[0] == 27000 || dJoyState.rgdwPOV[0] == 31500)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_STICK_L_UP:		// L�X�e�B�b�N��
						if (100 < dJoyState.lY)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_STICK_L_LEFT:	// L�X�e�B�b�N��
						if (dJoyState.lX < -100)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_STICK_L_RIGHT:	// L�X�e�B�b�N�E
						if (100 < dJoyState.lX)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_STICK_L_DOWN:	// L�X�e�B�b�N��
						if (dJoyState.lY < -100)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_STICK_R_UP:		// R�X�e�B�b�N��
						if (100 < dJoyState.lRz)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_STICK_R_LEFT:	// R�X�e�B�b�N��
						if (dJoyState.lZ < -100)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_STICK_R_RIGHT:	// R�X�e�B�b�N�E
						if (100 < dJoyState.lZ)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					case DIPADOTHER_STICK_R_DOWN:	// R�X�e�B�b�N��
						if (dJoyState.lRz < -100)
							bButton = 0x80;
						else
							bButton = 0;
						break;
					}
				}

				// �X�e�B�b�N����
				m_LStickRot = atan2f((float)dJoyState.lX, (float)dJoyState.lY);
				m_RStickRot = atan2f((float)dJoyState.lZ, (float)dJoyState.lRz);

				// �W���C�p�b�h�̓��͏��
				m_aJoyStateRelease[nCntJoyPad] = (m_aJoyStatePress[nCntJoyPad] ^ bButton) & m_aJoyStatePress[nCntJoyPad];
				m_aJoyStateTrigger[nCntJoyPad] = (m_aJoyStatePress[nCntJoyPad] ^ bButton) & bButton;
				m_aJoyStatePress[nCntJoyPad] = bButton;
			}
		}
		else
		{
			m_pDevice->Acquire();		// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
		}
	}
}

//=============================================================================
// DI�p�b�h�̓��͏��(�v���X���)���擾
//=============================================================================
bool CInputDIPad::GetPress(int nButton)
{
	return (m_aJoyStatePress[nButton] & 0x80) ? true : false;
}

//=============================================================================
// DI�p�b�h�̓��͏��(�g���K�[���)���擾
//=============================================================================
bool CInputDIPad::GetTrigger(int nButton)
{
	return (m_aJoyStateTrigger[nButton] & 0x80) ? true : false;
}

//=============================================================================
// DI�p�b�h�̓��͏��(�����[�X���)���擾\
//=============================================================================
bool CInputDIPad::GetRelease(int nButton)
{
	return (m_aJoyStateRelease[nButton] & 0x80) ? true : false;
}

//=============================================================================
// DI�p�b�h�̓��͏��(�X�e�B�b�N���)���擾	\
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
// DI�p�b�h�̓��͏��(�X�e�B�b�N�������)���擾
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
// �X�e�B�b�N�̎�X
//=============================================================================
float CInputDIPad::GetDRightXAxis(void)
{
	return m_RStickRotX;
}

//=============================================================================
// �X�e�B�b�N�̎�Y
//=============================================================================
float CInputDIPad::GetDRightYAxis(void)
{
	return m_RStickRotY;
}
//=============================================================================
// X�p�b�h�̎����͏��(�X�e�B�b�N�������)���擾
//=============================================================================
float  CInputDIPad::GetDRightAxis(void)
{
	float fAxis = atan2f(m_RStickRotX, m_RStickRotY);
	return fAxis;
}

//----------------------------------------------------------------------------------------------------
int CInputXPad::m_nInputNum = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputXPad::CInputXPad()
{
	// �����l�ݒ�
	for (int nCntCont = 0; nCntCont < MAX_CONTROLLERS; nCntCont++)
	{
		m_LStickRot[nCntCont] = 0.0f;		// L�X�e�B�b�N
		m_RStickRot[nCntCont] = 0.0f;		// R�X�e�B�b�N
	}

	m_nInputNum = 0;						// ���͐�
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInputXPad::~CInputXPad()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInputXPad::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �R���g���[���[�̏�����
	ZeroMemory(m_Controllers, sizeof(CONTROLER_STATE) * MAX_CONTROLLERS);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInputXPad::Uninit(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void CInputXPad::Update(void)
{
	UpdateControllerState();	// �r������

	for (DWORD nCntCont = 0; nCntCont < MAX_CONTROLLERS; nCntCont++)
	{// �R���g���[���[�J�E���g
		if (m_Controllers[nCntCont].bConnected == true)
		{// �ڑ�����Ă�����
			if (m_bDeadZoneOn == true)
			{// �X�e�B�b�N
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

			WORD wButtons = 0;		// XINPUT�R���g���[���[�̓��͏��

			for (int nCount = 0; nCount < XPADOTHER_MAX; nCount++)
			{// ���͏��J�E���g
				switch (nCount)
				{
				case 0:		// �{�^��
					wButtons = m_Controllers[nCntCont].state.Gamepad.wButtons;
					break;
				case 1:		// ���g���K�[
					wButtons = m_Controllers[nCntCont].state.Gamepad.bLeftTrigger;
					break;
				case 2:		// �E�g���K�[
					wButtons = m_Controllers[nCntCont].state.Gamepad.bRightTrigger;
					break;
				case 3:		// ���X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLY > XINPUT_STICK_MAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 4:		// ���X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLX < XINPUT_STICK_MIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 5:		// ���X�e�B�b�N�E
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLX > XINPUT_STICK_MAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 6:		// ���X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbLY < XINPUT_STICK_MIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 7:		// �E�X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRY > XINPUT_STICK_MAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 8:		// �E�X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRX < XINPUT_STICK_MIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 9:		// �E�X�e�B�b�N�E
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRX > XINPUT_STICK_MAX * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				case 10:		// �E�X�e�B�b�N��
					if (m_Controllers[nCntCont].state.Gamepad.sThumbRY < XINPUT_STICK_MIN * 0.1f)
						wButtons = 0x80;
					else
						wButtons = 0;
					break;
				}

				// �X�e�B�b�N����
				m_LStickRot[nCntCont] = atan2f((float)m_Controllers[nCntCont].state.Gamepad.sThumbLX, (float)m_Controllers[nCntCont].state.Gamepad.sThumbLY);
				m_RStickRot[nCntCont] = atan2f((float)m_Controllers[nCntCont].state.Gamepad.sThumbRX, (float)m_Controllers[nCntCont].state.Gamepad.sThumbRY);

				// �W���C�p�b�h�̓��͏��
				m_aJoyStateTrigger[nCntCont][nCount] = (m_aJoyStatePress[nCntCont][nCount] ^ wButtons) & wButtons;
				m_aJoyStateRelease[nCntCont][nCount] = (m_aJoyStatePress[nCntCont][nCount] ^ wButtons) & m_aJoyStatePress[nCntCont][nCount];
				m_aJoyStatePress[nCntCont][nCount] = wButtons;
			}
		}
	}
}

//=============================================================================
// X�p�b�h�̓��͏��(���͊m�F)���擾
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
// X�p�b�h�̓��͏��(���͐����)���擾
//=============================================================================
int CInputXPad::GetInputNum(void)
{
	return m_nInputNum;
}

//=============================================================================
// X�p�b�h�̓��͏��(�I�[�����)���擾
//=============================================================================
bool CInputXPad::GetALL(int nType, int nIdxPad)
{
	if (m_Controllers[nIdxPad].bConnected)
	{
		switch (nType)
		{
		case 0:
			for (int nCount = 0; nCount < XPADOTHER_MAX; nCount++)
			{// ���͏��J�E���g
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
			{// ���͏��J�E���g
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
			{// ���͏��J�E���g
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
// X�p�b�h�̓��͏��(�v���X���)���擾
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
// X�p�b�h�̓��͏��(�g���K�[���)���擾
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
// X�p�b�h�̓��͏��(�����[�X���)���擾
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
// X�p�b�h�̓��͏��(�X�e�B�b�N���)���擾
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
// X�p�b�h�̓��͏��(�X�e�B�b�N�������)���擾
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
// �X�e�B�b�N�̎�X
//=============================================================================
float CInputXPad::GetRightXAxis(void)
{
	return m_RStickRotX;
}

//=============================================================================
// �X�e�B�b�N�̎�Y
//=============================================================================
float CInputXPad::GetRightYAxis(void)
{
	return m_RStickRotY;
}
//=============================================================================
// X�p�b�h�̎����͏��(�X�e�B�b�N�������)���擾
//=============================================================================
float  CInputXPad::GetRightAxis(void)
{
	float fAxis = atan2f(m_RStickRotY, m_RStickRotX);
	return fAxis;
}