//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
// Author :YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "fade.h"			// �t�F�[�h
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_NAME000		"data\\TEXTURE\\FADE\\fade000.png"		// �e�N�X�`��
#define TEXTURE_NAME001		"data\\TEXTURE\\FADE\\fade001.png"		// �e�N�X�`��
#define ADDCOLOR_A			(1.0f / 60.0f)							// �t�F�[�h����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9			CFade::m_pTexture[MAX_TEX] = {};				// �e�N�X�`�����ւ̃|�C���^
CFade::FADE					CFade::m_fade			   = CFade::FADE_NONE;	// �t�F�[�h���

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CFade::CFade() : CScene2D(CScene::PRIORITY_FADE, CScene::OBJTYPE_FADE)
{
	// �����l�ݒ�
	m_nCntTimer = 0;		// �^�C�}�[
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
// ���[�h����
//=============================================================================
HRESULT CFade::Load(void)
{
		// �f�o�C�X�擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
		// �e�N�X�`���̐���
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME000, &m_pTexture[0]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME001, &m_pTexture[1]);

	return S_OK;
}

//=============================================================================
// �A�����[�h����
//=============================================================================
void CFade::Unload(void)
{
	for (int nCount = 0; nCount < MAX_TEX; nCount++)
	{//	�e�N�X�`���̍ő吔
		// �e�N�X�`���̔j��
		if (m_pTexture[nCount] != NULL)
		{// NULL�ȊO�̏ꍇ
			m_pTexture[nCount]->Release();		// ���
			m_pTexture[nCount] = NULL;			// NULL��
		}
	}
}

//=============================================================================
// �m�ۏ���
//=============================================================================
CFade *CFade::Create(CManager::MODE mode, FADE fade, int nType)
{
	CFade *pFade = NULL;					// �|�C���^

	if (m_fade == FADE_NONE)
	{// �t�F�[�h���Ă��Ȃ�
		if (pFade == NULL)
		{// NULL�̏ꍇ
			pFade = new CFade;				// �������m��
			if (pFade != NULL)
			{// NULL�ȊO�̏ꍇ
				pFade->Init(mode, fade);				// ����������
				if (nType < MAX_TEX)
				{// �e�N�X�`���ݒ�
					pFade->BindTexture(m_pTexture[nType]);
				}
				else
				{// �e�N�X�`���ݒ�
					pFade->BindTexture(NULL);
				}
			}
		}
	}
	return pFade;	// �l��Ԃ�
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CFade::Init(CManager::MODE mode, FADE fade)
{
	if (fade == FADE_OUT)
	{// �t�F�[�h�A�E�g
		CScene2D::SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));		// �����l�ݒ�
	}
	else
	{// ���̑�
		CScene2D::SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));		// �����l�ݒ�
	}

	CScene2D::Init();		// ����������

	m_fade = FADE_OUT;		// �t�F�[�h�A�E�g
	m_modeNext = mode;		// �����[�h
	m_nCntTimer = 0;		// �J�E���g

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CFade::Uninit(void)
{
	CScene2D::Uninit();						// �I������
}

//=============================================================================
// �X�V����
//=============================================================================
void CFade::Update(void)
{
	D3DXCOLOR col = CScene2D::Getcol();		// �F���擾
	bool bDelete = false;					// �j������

	if (m_fade != FADE_NONE)
	{// �t�F�[�h�g���Ă���
		if (m_fade == FADE_IN)
		{// �t�F�[�h�C��
			col.a -= ADDCOLOR_A;			// ������

			if (col.a <= 0.0f)
			{// ����
				col.a = 0.0f;				// �����x�𒲐�
				m_fade = FADE_NONE;			// �������Ă��Ȃ����
				m_nCntTimer = 0;			// �J�E���g������

				bDelete = true;				// �j������
			}
		}
		else if (m_fade == FADE_OUT)
		{// �t�F�[�h�A�E�g
			col.a += ADDCOLOR_A;			// �s������

			if (col.a >= 1.0f)
			{// �s����
				col.a = 1.0f;				// �����x�𒲐�

				m_nCntTimer++;				// �J�E���^�����Z
				if (m_nCntTimer > 30)
				{// ���t���[����
					m_fade = FADE_IN;		// �t�F�[�h�C��
				}
				else if (m_nCntTimer == 15)
				{// �t���[���̒���
					CManager::SetMode(m_modeNext);		// ���[�h�ύX
					CScene::SetStartUp(0);				// �X�V�͈͂�������
				}
			}
		}
	}

	CScene2D::Setcol(col);					// �F��ݒ�
	CScene2D::Update();						// �X�V����

	if (bDelete == true)
	{// �j������
		Uninit();							// �I������
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CFade::Draw(void)
{
	CScene2D::Draw();						// �`�揈��
}