//=============================================================================
//
// UI���� [UIe.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "ui.h"				// �^�C�g���t���[��
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "game.h"			// �Q�[��
#include "player.h"			//	�v���C���[
#include "fade.h"			//	�t�F�C�h
#include "sound.h"			//	�T�E���h

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//	#	�^�C�g��
#define	TEXTURE_TITLE000		"data\\TEXTURE\\UI\\TITLE\\title000.png"		// �^�C�g�����S000
#define	TEXTURE_BOTTON_ENTER	"data\\TEXTURE\\UI\\BUTTON\\PRESSENTER.png"		// �{�^��
//	#	�`���[�g���A��
#define	TEXTURE_TUTORIAL000		"data\\TEXTURE\\UI\\TUTORIAL\\tutorial.png"		// �`���[�g���A��
//	#	�Q�[��
#define	TEXTURE_GAME000		"data\\TEXTURE\\UI\\GAME\\game000.png"				// LIVE
#define	TEXTURE_GAME001		"data\\TEXTURE\\UI\\GAME\\game001.png"				// �V�N��
#define	TEXTURE_GAME002 	"data\\TEXTURE\\UI\\GAME\\game002.png"				// ����
#define	TEXTURE_GAME003 	"data\\TEXTURE\\UI\\GAME\\Blood.png"				// �Ԃ茌
#define	TEXTURE_GAME004 	"data\\TEXTURE\\UI\\GAME\\nengou000.jpg"			// �N��000
//	#	�����L���O
#define	TEXTURE_RANKING001 	"data\\TEXTURE\\UI\\RANKING\\raking001.png"			// ����001
#define	TEXTURE_RANKING002 	"data\\TEXTURE\\UI\\RANKING\\raking002.png"			// ����002
#define	TEXTURE_RANKING003 	"data\\TEXTURE\\UI\\RANKING\\raking003.png"			// ����003
#define	TEXTURE_RANKING004 	"data\\TEXTURE\\UI\\RANKING\\raking004.png"			// ����004
#define	TEXTURE_RANKING005 	"data\\TEXTURE\\UI\\RANKING\\raking005.png"			// ����005



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	CUI::m_pTexture[UI_TEXMAX] = {};			//  �e�N�X�`�����ւ̃|�C���^
CManager::MODE		CUI::m_mode = CManager::MODE_NONE;			//	�Q�[���̃��[�h

																//=============================================================================
																// �R���X�g���N�^
																//=============================================================================
CUI::CUI() : CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_UI)
{
	m_nType = 0;								//	���
	m_sizeOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//	�����̑傫���̈ʒu
	m_bFlash = false;							//	�_�ł��Ȃ�
	m_bPressButton = false;					//	�{�^���̐ݒ�
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CUI::~CUI()
{

}
//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CUI::Load(CManager::MODE mode)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//	�Q�[���̐ݒ�
	m_mode = mode;
	switch (mode)
	{
	case CManager::MODE_TITLE://	�^�C�g��	------------------------------------------------------------------------
		D3DXCreateTextureFromFile(pDevice, TEXTURE_BOTTON_ENTER, &m_pTexture[0]);	//	�{�^��
		D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE000, &m_pTexture[1]);		//	�^�C�g�����S
		break;
	case CManager::MODE_TUTORIAL://	�`���[�g���A��	---------------------------------------------------------------------
		D3DXCreateTextureFromFile(pDevice, TEXTURE_BOTTON_ENTER, &m_pTexture[0]);	//	�{�^��
		D3DXCreateTextureFromFile(pDevice, TEXTURE_TUTORIAL000, &m_pTexture[2]);	//	�`���[�g���A��
		break;
	case CManager::MODE_GAME://	�Q�[��	-----------------------------------------------------------------------------
		D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME000, &m_pTexture[3]);		// LIVE
		D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME001, &m_pTexture[4]);		// �V�N��
		D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME002, &m_pTexture[5]);		// ����
		D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME003, &m_pTexture[6]);		// �Ԃ茌
		D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME004, &m_pTexture[7]);		// �N��

		break;
	case CManager::MODE_RANKING://	�����L���O	-------------------------------------------------------------------------
		D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING001, &m_pTexture[8]);		// ����001
		D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING002, &m_pTexture[9]);		// ����002
		D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING003, &m_pTexture[10]);	// ����003
		D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING004, &m_pTexture[11]);	// ����004
		D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING005, &m_pTexture[12]);	// ����005


		break;
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h����
//=============================================================================
void CUI::Unload(void)
{
	for (int nCntTex = 0; nCntTex < UI_TEXMAX; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{// NULL�̏ꍇ
		 // �e�N�X�`���̐���
			m_pTexture[nCntTex]->Release();		// ���
			m_pTexture[nCntTex] = NULL;			// NULL��
		}
	}
}

//=============================================================================
// ��������
//=============================================================================
CUI *CUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 uv, int nType)
{
	CUI *pUI = {};				// Ui�|�C���^

	if (pUI == NULL)
	{// NULL�̏ꍇ// �������m��
		pUI = new CUI;
		if (pUI != NULL)
		{// NULL�ȊO�̏ꍇ
			pUI->Init(pos, size, col, uv, nType);			// ����������
		}
	}
	return pUI;
}

//=============================================================================
// UI�쐬���郂�[�h
//=============================================================================
void CUI::CreateMode(CManager::MODE mode)
{
	m_mode = mode;	//���[�h�̓���
	switch (mode)
	{
	case CManager::MODE_TITLE://	�^�C�g��	------------------------------------------------------------------------
							  //	�^�C�g�����S000
		CUI::Create
		(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50, 0.0f), D3DXVECTOR3(650.0f, 320.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 1);
		//	PRESS ENTER
		CUI::Create
		(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXVECTOR3(200.0f, 50.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 0);
		break;
	case CManager::MODE_TUTORIAL://	�`���[�g���A��	---------------------------------------------------------------------
								 //	�`���[�g���A���w�i
		CUI::Create
		(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 2);
		//	PRESS ENTER
		CUI::Create
		(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXVECTOR3(200.0f, 50.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 0);
		break;
	case CManager::MODE_GAME://	�Q�[��	-----------------------------------------------------------------------------
							 //	LIVE
		CUI::Create
		(D3DXVECTOR3(100, 50, 0.0f), D3DXVECTOR3(90, 40, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 3);
		// �V�N��
		CUI::Create
		(D3DXVECTOR3(SCREEN_WIDTH / 2, 670, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2 - 50, 60, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 4);
		// ����
		CUI::Create
		(D3DXVECTOR3(100, 570, 0.0f), D3DXVECTOR3(90, 40, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 5);
		// �Ԃ茌
		CUI::Create
		(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 6);
		// �N��
		CUI::Create
		(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50, 0.0f), D3DXVECTOR3(0.0f, 100, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 7);
		break;
	case CManager::MODE_RANKING://	�����L���O	-------------------------------------------------------------------------
		// ���� 1��
		CUI::Create
		(D3DXVECTOR3(500.0f, 150.0f, 0.0f), D3DXVECTOR3(70.0f, 60, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 8);

		// ���� 2��
		CUI::Create
		(D3DXVECTOR3(600.0f, 280.0f, 0.0f), D3DXVECTOR3(80.0f, 60, 0.0f),
			D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 9);

		// ���� 3��
		CUI::Create
		(D3DXVECTOR3(680.0f, 400.0f, 0.0f), D3DXVECTOR3(50.0f, 40, 0.0f),
			D3DXCOLOR(0.7f, 0.5f, 0.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 10);

		// ���� 4��
		CUI::Create
		(D3DXVECTOR3(680.0f, 500.0f, 0.0f), D3DXVECTOR3(50.0f, 40, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 11);

		// ���� 5��
		CUI::Create
		(D3DXVECTOR3(680.0f, 600.0f, 0.0f), D3DXVECTOR3(50.0f, 40, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 12);
		break;
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CUI::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 uv, int nType)
{
	CScene2D::SetInitAll(pos, size, col, uv);	// �����l�ݒ�
	CScene2D::Init();							// ����������
	CScene2D::BindTexture(m_pTexture[nType]);	// �e�N�X�`�����蓖��
												//	��ނ̐ݒ�
	m_nType = nType;
	//	�̂̑傫���̐ݒ�
	m_sizeOld = size;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CUI::Uninit(void)
{
	CScene2D::Uninit();						// �I������
}

//=============================================================================
// �X�V����
//=============================================================================
void CUI::Update(void)
{
	D3DXVECTOR3 pos = CScene2D::Getpos();	//	�ʒu�̎擾
	D3DXVECTOR3 size = CScene2D::Getsize();	//	�傫���̎擾
	D3DXCOLOR col = CScene2D::Getcol();		//	�F�̎擾

	m_nCounter++;							//	�J�E���^�[

											// �X�V����
	CScene2D::Update();
	switch (m_mode)
	{
	case CManager::MODE_TITLE:

		if (m_nType == 0)
		{//	�v���X�G���^�[��������
			if (CFade::GetFade() == CFade::FADE_NONE)
			{// �t�F�[�h���Ă��Ȃ����
				if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
				{// �L�[�{�[�h�iENTER�j����͂�����
					m_bPressButton = true;
				}

			}
			if (m_bPressButton == false)
			{//	�{�^���������Ă��Ȃ�
				if ((m_nCounter % 40) == 0)
				{//	60�b�œ_��
					m_bFlash = m_bFlash ? false : true;

				}
			}
			else if (m_bPressButton == true)
			{//	�{�^����������
				if ((m_nCounter % 6) == 0)
				{//	60�b�œ_��
					m_bFlash = m_bFlash ? false : true;

				}
			}
			if (m_bFlash == true)
			{//	�T�C�Y���Ȃ���
				size.x = 0;
			}
			else if (m_bFlash == false)
			{//	�T�C�Y�������̑�؂���C�߂�
				size.x = 200.0f;
			}
		}
		break;
		//-----------------------------
		//	�`���[�g���A��
		//------------------------------
	case CManager::MODE_TUTORIAL:
		if (m_nType == 0)
		{//	�v���X�G���^�[��������
			if (CFade::GetFade() == CFade::FADE_NONE)
			{// �t�F�[�h���Ă��Ȃ����
				if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
				{// �L�[�{�[�h�iENTER�j����͂�����
					m_bPressButton = true;
				}

			}
			if (m_bPressButton == false)
			{//	�{�^���������Ă��Ȃ�
				if ((m_nCounter % 40) == 0)
				{//	60�b�œ_��
					m_bFlash = m_bFlash ? false : true;

				}
			}
			else if (m_bPressButton == true)
			{//	�{�^����������
				if ((m_nCounter % 6) == 0)
				{//	60�b�œ_��
					m_bFlash = m_bFlash ? false : true;

				}
			}
			if (m_bFlash == true)
			{//	�T�C�Y���Ȃ���
				size.x = 0;
			}
			else if (m_bFlash == false)
			{//	�T�C�Y�������̑�؂���C�߂�
				size.x = 200.0f;
			}
		}
		break;
		//-----------------------------
		//	�Q�[�����[�h
		//------------------------------
	case CManager::MODE_GAME:
		if (m_nType == 3)
		{//	LIVE
			if ((m_nCounter % 40) == 0)
			{//	60�b�œ_��
				m_bFlash = m_bFlash ? false : true;

			}
			if (m_bFlash == true)
			{//	�T�C�Y���Ȃ���
				size.x = 0;
			}
			else if (m_bFlash == false)
			{//	�T�C�Y�������̑�؂���C�߂�
				size.x = 90.0f;
			}
		}
		if (m_nType == 6)
		{//	�Ԃ茌
			if (CPlayer::GetDisp() == true)
			{// �L�[�{�[�h�iENTER�j����͂�����
				size.x = SCREEN_WIDTH / 2;
			}
		}
		if (m_nType == 7)
		{//	�N��
			if (CPlayer::GetNengouUp() == true)
			{// �L�[�{�[�h�iENTER�j����͂�����
				size.x = 80.0f;
			}
			else if (CPlayer::GetNengouUp() == false)
			{// �L�[�{�[�h�iENTER�j����͂�����
				size.x = 0.0f;
			}
		}
		break;
	}

	CScene2D::Setpos(pos);				//	�ʒu�̐ݒ�
	CScene2D::Setsize(size);			//	�傫���̐ݒ�
	CScene2D::Setcol(col);				//	�F�̐ݒ�
}

//=============================================================================
// �`�揈��
//=============================================================================
void CUI::Draw(void)
{
	CScene2D::Draw();						// �`�揈��
}


