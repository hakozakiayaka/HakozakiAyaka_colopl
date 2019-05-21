//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "camera.h"			// �J����
#include "light.h"			// ���C�g
#include "renderer.h"		// �����_���[
#include "fade.h"			// �t�F�[�h
#include "title.h"			// �^�C�g��
#include "game.h"			// �Q�[��
#include "result.h"			// ���U���g
#include "ranking.h"		// �����L���O
#include "scene.h"			// �V�[��
#include "debugproc.h"		// �f�o�b�O���O
#include "sound.h"			// �T�E���h
#include "tutorial.h"		// �`���[�g���A��

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define STATE_MODE (MODE_TITLE)		//	�n�܂郂�[�h

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CManager::MODE		CManager::m_mode			= CManager::MODE_TITLE;	// ���[�h
CInputKeyboard		*CManager::m_pInputKeyboard = NULL;					// �L�[�{�[�h
CInputDIPad			*CManager::m_pInputDIPad	= NULL;					// DI�p�b�h
CInputXPad			*CManager::m_pInputXPad		= NULL;					// X�p�b�h
CRenderer			*CManager::m_pRenderer		= NULL;					// �����_���[
CCamera				*CManager::m_pCamara		= NULL;					// �J����
CLight				*CManager::m_pLight			= NULL;					// ���C�g
CTitle				*CManager::m_pTitle			= NULL;					// �^�C�g��
CTutorial			*CManager::m_pTutorial		= NULL;					// �`���[�g���A��
CSelectMode			*CManager::m_pSelectMode	= NULL;					// �Z���N�g���[�h
CGame				*CManager::m_pGame			= NULL;					// �Q�[��
CResult				*CManager::m_pResult		= NULL;					// ���U���g
CDebugProcLEFT *CManager::m_CDebugProcLeft		= NULL;					// �f�o�b�O�p�̍��p������
CDebugProcRIGHT *CManager::m_CDebugProcRight	= NULL;					// �f�o�b�O�p�̉E�p������
CSound				*CManager::m_pSound			= NULL;					// �T�E���h
int					CManager::m_nPlayerLife = 0;						//	�v���C���[��HP

//�����L���O�ւ��鏈��
CRanking			*CManager::m_pRanking = NULL;					// ����

int CManager::nRank[5] = { 200,300,400,500,600 };
int CManager::m_LastScore = 0;//�����X�R�A(��{��0�X�^�[�g��)
int CManager::m_LastTime = 999999 * 1;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CManager::CManager()
{
	m_nPlayerLife = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// �L�[�{�[�h
		if (m_pInputKeyboard == NULL)
		{// NULL�̏ꍇ//// �L�[�{�[�h�������m��
			m_pInputKeyboard = new CInputKeyboard;
			if (m_pInputKeyboard != NULL)
			{// NULL�ȊO�̏ꍇ
			 // �L�[�{�[�h����������
				if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
				{
					return -1;
				}
			}
			else
			{// NULL�̏ꍇ
				MessageBox(0, "InputKeyboard��NULL", "�x��", MB_OK);
			}
		}
		else
		{// NULL�ȊO�̏ꍇ
			MessageBox(0, "InputKeyboard��NULL�łȂ�", "�x��", MB_OK);
		}
	// DI�p�b�h
		if (m_pInputDIPad == NULL)
		{// NULL�̏ꍇ// DI�p�b�h�������m��
			m_pInputDIPad = new CInputDIPad;
			if (m_pInputDIPad != NULL)
			{// NULL�ȊO�̏ꍇ
			 // DI�p�b�h����������
				if (FAILED(m_pInputDIPad->Init(hInstance, hWnd)))
				{
					return -1;
				}
			}
			else
			{// NULL�̏ꍇ
				MessageBox(0, " InputDIPad��NULL", "�x��", MB_OK);
			}
		}
		else
		{// NULL�ȊO�̏ꍇ
			MessageBox(0, "InputDIPad��NULL�łȂ�", "�x��", MB_OK);
		}
	// DI�p�b�h
		if (m_pInputXPad == NULL)
		{// NULL�̏ꍇ// DI�p�b�h�������m��
			m_pInputXPad = new CInputXPad;
			if (m_pInputXPad != NULL)
			{// NULL�ȊO�̏ꍇ
			 // DI�p�b�h����������
				if (FAILED(m_pInputXPad->Init(hInstance, hWnd)))
				{
					return -1;
				}
			}
			else
			{// NULL�̏ꍇ
				MessageBox(0, "InputXPad��NULL", "�x��", MB_OK);
			}
		}
		else
		{// NULL�ȊO�̏ꍇ
			MessageBox(0, " InputXPad��NULL�łȂ�", "�x��", MB_OK);
		}
		// �����_���[
		if (m_pRenderer == NULL)
		{// NULL�̏ꍇ// �����_���[�������m��
			m_pRenderer = new CRenderer;
			if (m_pRenderer != NULL)
			{// NULL�ȊO�̏ꍇ
			 // �����_���[����������
				if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
				{
					return -1;
				}
			}
			else
			{// NULL�̏ꍇ,
				MessageBox(0, "Renderer��NULL", "�x��", MB_OK);
			}
		}
		else
		{// NULL�ȊO�̏ꍇ
			MessageBox(0, "Renderer��NULL�łȂ�", "�x��", MB_OK);
		}
		if (m_pSound == NULL)
		{//	�T�E���h���g�p����Ă��Ȃ������ꍇ
			m_pSound = new CSound;
			if (m_pSound != NULL)
			{//	�T�E���h�̐���
				m_pSound->InitSound(hWnd);
			}
			else
			{// NULL�̏ꍇ
				MessageBox(0, " �T�E���h��NULL", "�x��", MB_OK);
			}
		}
		else
		{// NULL�ȊO�̏ꍇ
			MessageBox(0, " �T�E���h��NULL�łȂ�", "�x��", MB_OK);
		}

		// ���C�g
		if (m_pLight == NULL)
		{// NULL�̏ꍇ// ���C�g�������m��
			m_pLight = new CLight;
			if (m_pLight != NULL)
			{// NULL�ȊO�̏ꍇ
			 // ���C�g����������
				if (FAILED(m_pLight->Init()))
				{
					return -1;
				}
			}
			else
			{// NULL�̏ꍇ
				MessageBox(0, " Light��NULL", "�x��", MB_OK);
			}
		}
		else
		{// NULL�ȊO�̏ꍇ
			MessageBox(0, " Light��NULL�łȂ�", "�x��", MB_OK);
		}
		if (m_CDebugProcLeft == NULL)
		{//	��������̐���
			m_CDebugProcLeft = new CDebugProcLEFT;
			if (m_CDebugProcLeft != NULL)
			{//	�������񂪐�������Ă���
				m_CDebugProcLeft->Init();
			}
		}
		if (m_CDebugProcRight == NULL)
		{//	�E������̐���
			m_CDebugProcRight = new CDebugProcRIGHT;
			if (m_CDebugProcRight != NULL)
			{//	�E�����񂪐�������Ă���
				m_CDebugProcRight->Init();
			}
		}
	//	�t�F�C�h�̓ǂݍ���
	CFade::Load();
	SetMode(STATE_MODE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void)
{

	switch (m_mode)
	{// ���[�h
	 // �^�C�g��----------------------------
	case CManager::MODE_TITLE:
		if (m_pTitle != NULL)
		{// �I������
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;
		// �`���[�g���A��----------------------------
	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// �I������
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;
		// �Q�[���X�e�[�W1----------------------------
	case CManager::MODE_GAME:
		if (m_pGame != NULL)
		{// �I������
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		break;
		// �����L���O----------------------------
	case CManager::MODE_RANKING:
		if (m_pRanking != NULL)
		{// �I������
			m_pRanking->Uninit();
			delete m_pRanking;
			m_pRanking = NULL;
			m_LastScore = 0;//�����L���O���I��莟��ēx���l�����Z�b�g
			m_LastTime = 999999 * 1;//�������^�C���̏�����

		}
		break;

	}
	//	�t�F�C�h�̊J��
	CFade::Unload();

	if (m_pInputKeyboard != NULL)
	{// NULL�ȊO�̏ꍇ
		// �I������
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;	// �L�[�{�[�h�������J��
		m_pInputKeyboard = NULL;	// NULL
	}

	if (m_pInputDIPad != NULL)
	{// NULL�ȊO�̏ꍇ
		// �I������
		m_pInputDIPad->Uninit();
		delete m_pInputDIPad;	// DI�p�b�h�������J��
		m_pInputDIPad = NULL;	// NULL
	}
	if (m_pInputXPad != NULL)
	{// NULL�ȊO�̏ꍇ
	 // �I������
		m_pInputXPad->Uninit();
		delete m_pInputXPad;	// X�p�b�h�������J��
		m_pInputXPad = NULL;	// NULL
	}

	// �����_���[
	if (m_pRenderer != NULL)
	{// NULL�ȊO�̏ꍇ
		// �I������
		m_pRenderer->Uninit();
		delete m_pRenderer;	// �����_���[�������J��
		m_pRenderer = NULL;	// NULL
	}

	// �J����
	if (m_pCamara != NULL)
	{// NULL�ȊO�̏ꍇ
		// �I������
		m_pCamara->Uninit();
		delete m_pCamara;	// �J�����������J��
		m_pCamara = NULL;	// NULL
	}
	if (m_pSound != NULL)
	{// NULL�ȊO�̏ꍇ
		//�I������
		m_pSound->UninitSound();
		delete m_pSound;	//	�T�E���h�̊J��
		m_pSound = NULL;	//	NULL
	}
	// ���C�g
	if (m_pLight != NULL)
	{// NULL�ȊO�̏ꍇ
		// �I������
		m_pLight->Uninit();

		delete m_pLight;	// ���C�g�������J��
		m_pLight = NULL;	// NULL
	}

	if (m_CDebugProcLeft != NULL)
	{
		//	�f�o�b�O�p�����t�H���g�̏I������
		m_CDebugProcLeft->Uninit();
		//�������̊J��
		delete m_CDebugProcLeft;
		//��ɂ���
		m_CDebugProcLeft = NULL;
	}
	if (m_CDebugProcRight != NULL)
	{
		//	�f�o�b�O�p�����t�H���g�̏I������
		m_CDebugProcRight->Uninit();
		//�������̊J��
		delete m_CDebugProcRight;
		//��ɂ���
		m_CDebugProcRight = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void)
{
	switch (m_mode)
	{// ���[�h
	 // �^�C�g��----------------------------
	case CManager::MODE_TITLE:
		if (m_pTitle != NULL)
		{// �X�V����
			m_pTitle->Update();
		}
		break;
		// �`���[�g���A��----------------------------
	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// �I������
			m_pTutorial->Update();
		}
		break;
		// �X�e�[�W1------------------------------
	case CManager::MODE_GAME:
		if (m_pGame != NULL)
		{// �X�V����
			m_pGame->Update();
		}
		break;
		// �����L���O------------------------------
	case CManager::MODE_RANKING:
		if (m_pRanking != NULL)
		{// �X�V����
			m_pRanking->Update();
		}
		break;

	}
	if (m_pInputKeyboard != NULL)
	{// NULL�ȊO�̏ꍇ
		// �L�[�{�[�h�X�V����
		m_pInputKeyboard->Update();
	}

	if (m_pInputDIPad != NULL)
	{// NULL�ȊO�̏ꍇ
		// DI�p�b�h�X�V����
		m_pInputDIPad->Update();
	}

	if (m_pInputXPad != NULL)
	{// NULL�ȊO�̏ꍇ
		// X�p�b�h�X�V����
		m_pInputXPad->Update();
	}

	if (m_pRenderer != NULL)
	{// NULL�ȊO�̏ꍇ
		// �����_���[�X�V����
		m_pRenderer->Update();
	}

	if (m_pCamara != NULL)
	{// NULL�ȊO�̏ꍇ
		// �J�����X�V����
		m_pCamara->Update();
	}

	if (m_pLight != NULL)
	{// NULL�ȊO�̏ꍇ
		// ���C�g�X�V����
		m_pLight->Update();
	}


}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	if (m_pCamara != NULL)
	{// NULL�ȊO�̏ꍇ
	 // �J�����`�揈��
		m_pCamara->SetCamera();
	}
	if (m_pRenderer != NULL)
	{// NULL�ȊO�̏ꍇ
	 // �����_���[�`�揈��
		m_pRenderer->Draw();
	}
}

//=============================================================================
// ���[�h�ݒ菈��
//=============================================================================
void CManager::SetMode(CManager::MODE mode)
{

	switch (m_mode)
	{// ���[�h�i���݁j
	 // �^�C�g��----------------------------
	case CManager::MODE_TITLE:
		if (m_pTitle != NULL)
		{// �I������
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_TITLE);	//	BGM�~�߂�
		// �J����
		if (m_pCamara != NULL)
		{// NULL�ȊO�̏ꍇ
		 // �I������
			m_pCamara->Uninit();
			delete m_pCamara;	// �J�����������J��
			m_pCamara = NULL;	// NULL
		}
		break;
		// �`���[�g���A��----------------------------
	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// �I������
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_TUTORIAL);
		break;
		// �X�e�[�W1---------------------------
	case CManager::MODE_GAME:
		if (m_pGame != NULL)
		{// �I������
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		// �J����
		if (m_pCamara != NULL)
		{// NULL�ȊO�̏ꍇ
		 // �I������
			m_pCamara->Uninit();
			delete m_pCamara;	// �J�����������J��
			m_pCamara = NULL;	// NULL
		}
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME);
		break;

		// �����L���O---------------------------
	case CManager::MODE_RANKING:
		if (m_pRanking != NULL)
		{// �I������
			m_pRanking->Uninit();
			delete m_pRanking;
			m_pRanking = NULL;
			m_LastScore = 0;//�����L���O���I��莟��ēx���l�����Z�b�g
			m_LastTime = 999999 * 1;//�������^�C���̏�����
		}
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_RANKING);
		// �J����
		if (m_pCamara != NULL)
		{// NULL�ȊO�̏ꍇ
		 // �I������
			m_pCamara->Uninit();
			delete m_pCamara;	// �J�����������J��
			m_pCamara = NULL;	// NULL
		}
		break;

	}


	// ���[�h�ύX
	m_mode = mode;

	switch (mode)
	{// ���[�h
	 // �^�C�g��----------------------------
	case CManager::MODE_TITLE:
		if (m_pTitle == NULL)
		{// NULL�̏ꍇ
			m_pTitle = new CTitle;
			if (m_pTitle != NULL)
			{// ����������
				m_LastTime = 999999 * 1;//�������^�C���̏�����

				m_pTitle->Init();
			}
		}
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_TITLE);	//	BGM�炷
		if (m_pCamara == NULL)
		{// NULL�̏ꍇ// �J�����������m��
			m_pCamara = new CCamera;
			if (m_pCamara != NULL)
			{// NULL�ȊO�̏ꍇ
			 // �J��������������
				m_pCamara->Init(CCamera::CAMERATYPE_TITLE);
			}
		}
		break;
		// �`���[�g���A��----------------------------
	case MODE_TUTORIAL:
		if (m_pTutorial == NULL)
		{// �I������
			m_pTutorial = new CTutorial;
			if (m_pTutorial != NULL)
			{// ����������
				m_pTutorial->Init();
			}
		}
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_TUTORIAL);	//	BGM�炷
		break;
		// �X�e�[�W1----------------------------
	case CManager::MODE_GAME:
		// �J����
		if (m_pGame == NULL)
		{// NULL�̏ꍇ
			m_pGame = new CGame;
			if (m_pGame != NULL)
			{// ����������
				m_pGame->Init(CGame::STAGE_001, m_nPlayerLife);
			}
		}
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);	//	BGM�炷
		if (m_pCamara == NULL)
		{// NULL�̏ꍇ// �J�����������m��
			m_pCamara = new CCamera;
			if (m_pCamara != NULL)
			{// NULL�ȊO�̏ꍇ
			 // �J��������������
				m_pCamara->Init(CCamera::CAMERATYPE_GAME);
			}
		}
		break;

		// �����L���O----------------------------
	case CManager::MODE_RANKING:
		// �J����
		if (m_pRanking == NULL)
		{// NULL�̏ꍇ
			m_pRanking = new CRanking;
			if (m_pRanking != NULL)
			{// ����������
				m_pRanking->Init();
			}
		}
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_RANKING);
		if (m_pCamara == NULL)
		{// NULL�̏ꍇ// �J�����������m��
			m_pCamara = new CCamera;
			if (m_pCamara != NULL)
			{// NULL�ȊO�̏ꍇ
			 // �J��������������
				m_pCamara->Init(CCamera::CAMERATYPE_RANKING);
			}
		}
		break;
	}
}
