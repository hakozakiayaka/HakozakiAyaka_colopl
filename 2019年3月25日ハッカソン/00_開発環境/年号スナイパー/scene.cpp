//=============================================================================
//
// �V�[������ [scene.cpp]
// Author :YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene.h"		// �V�[��
#include "manager.h"	// �}�l�[�W���[
#include "renderer.h"	// �����_���[
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
CScene		*CScene::m_pTop[PRIORITY_MAX] = {};			// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
CScene		*CScene::m_pCur[PRIORITY_MAX] = {};			// ���݂̃I�u�W�F�N�g�ւ̃|�C���^

int			CScene::m_nNumAll = 0;						// ����
int			CScene::m_nNumPri[PRIORITY_MAX] = {};		// ����
int			CScene::m_nStartUp = 0;						// �X�V�͈�

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene::CScene(int nPriority, OBJTYPE objType)
{
	m_pPrev = m_pCur[nPriority];						// �O�̃I�u�W�F�N�g�ݒ�
	m_pNext = NULL;										// ���̃I�u�W�F�N�g�ݒ�

	m_pCur[nPriority] = this;							// ���݂̃I�u�W�F�N�g�ݒ�

	if (m_pPrev != NULL)
	{// �O�̃I�u�W�F�N�g��NULL�ȊO�̏ꍇ
		m_pPrev->m_pNext = m_pCur[nPriority];			// �O�̎��̃I�u�W�F�N�g�ݒ�
	}

	m_nPriority = nPriority;							// ���ʐݒ�
	m_objType = objType;								// ���
	m_bDeath = false;									// ���S�t���O
	m_nNumAll++;										// �����J�E���g
	m_nNumPri[nPriority]++;								// �����J�E���g

	if (m_pTop[nPriority] == NULL)
	{// �擪�̃I�u�W�F�N�g��NULL�ȊO�̏ꍇ
		m_pTop[nPriority] = m_pCur[nPriority];			// �擪�̃I�u�W�F�N�g�ݒ�
	}
	else if (m_pTop[nPriority] == m_pCur[nPriority]->m_pPrev)
	{// �擪�̃I�u�W�F�N�g�ƑO�̃I�u�W�F�N�g�������ꍇ
		m_pTop[nPriority]->m_pNext = m_pCur[nPriority];	// �擪�̎��̃I�u�W�F�N�g�ݒ�
	}

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// �S�t�F�[�h�ȊO�J������
//=============================================================================
void CScene::ReleaseMode(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_FADE; nCntPriority++)
	{// �J�E���g
		while (m_pTop[nCntPriority] != NULL)
		{// �擪�̃I�u�W�F�N�g��NULL�ɂȂ�܂�
			m_pTop[nCntPriority]->Uninit();			// �擪�̃I�u�W�F�N�g�I��
			m_pTop[nCntPriority]->Release();		// �J������
		}
		m_nNumPri[nCntPriority] = 0;				// �v���C�I���e�B����������
	}

	m_nNumAll = 0;									// ����������
}

//=============================================================================
// �S�J������
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{// �J�E���g
		while (m_pTop[nCntPriority] != NULL)
		{// �擪�̃I�u�W�F�N�g��NULL�ɂȂ�܂�
			m_pTop[nCntPriority]->Uninit();			// �擪�̃I�u�W�F�N�g�I��
			m_pTop[nCntPriority]->Release();		// �J������
		}
		m_nNumPri[nCntPriority] = 0;				// �v���C�I���e�B����������
	}

	m_nNumAll = 0;									// ����������
}

//=============================================================================
// �S�X�V����
//=============================================================================
void CScene::UpadteAll(void)
{
	for (int nCntPriority = m_nStartUp; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CScene *pScene = m_pTop[nCntPriority];		// �X�V�̃I�u�W�F�N�g�i�擪���L���j
		CScene *pNext = NULL;						// ���̃I�u�W�F�N�g�L��

		while (pScene != NULL)
		{// �X�V�̃I�u�W�F�N�g��NULL�ȊO�̏ꍇ
			pNext = pScene->m_pNext;				// ���̃I�u�W�F�N�g�ݒ�
			if (pScene->m_bDeath == false)
			{// �j�����Ȃ�
				pScene->Update();					// �X�V����
			}
			if (pScene->m_bDeath == true)
			{// �j������
				pScene->Release();					// �J������
			}
			pScene = pNext;							// �X�V�̃I�u�W�F�N�g�Ɏ��̃I�u�W�F�N�g�ݒ�
		}
	}
}

//=============================================================================
// �S�`�揈��
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{// �J�E���g
		CScene *pScene = m_pTop[nCntPriority];		// �`��̃I�u�W�F�N�g�i�擪���L���j
		CScene *pNext = NULL;						// ���̃I�u�W�F�N�g�L��

		while (pScene != NULL)
		{// �`��̃I�u�W�F�N�g��NULL�ȊO�̏ꍇ
			pNext = pScene->m_pNext;				// ���̃I�u�W�F�N�g�ݒ�
			if (pScene->m_bDeath == false)
			{// �j�����Ȃ�
				pScene->Draw();						// �`�揈��
			}
			if (pScene->m_bDeath == true)
			{// �j������
				pScene->Release();					// �J������
			}
			pScene = pNext;							// �`��̃I�u�W�F�N�g�Ɏ��̃I�u�W�F�N�g�ݒ�
		}
	}
}

//=============================================================================
// �J������
//=============================================================================
void CScene::Release(void)
{
	if (this != NULL)
	{// ������NULL�ȊO�̏ꍇ
		int nPriority = m_nPriority;
		OBJTYPE objType = m_objType;

		if (m_pPrev != NULL)	// m_pTop�ȊO
		{// �O�̃I�u�W�F�N�g��NULL�ȊO�̏ꍇ
			if (m_pTop[nPriority] == m_pPrev)
			{// �擪�̃I�u�W�F�N�g���O�̃I�u�W�F�N�g�Ɠ����ꍇ
				m_pTop[nPriority]->m_pNext = m_pNext;		// �擪�̎��̃I�u�W�F�N�g�ݒ�
			}

			m_pPrev->m_pNext = m_pNext;						// �O�̎��̃I�u�W�F�N�g�ݒ�
		}

		if (m_pNext != NULL)	// m_pCur�ȊO
		{// ���̃I�u�W�F�N�g��NULL�ȊO�̏ꍇ
			if (m_pCur[nPriority] == m_pNext)
			{// ���݂̃I�u�W�F�N�g���O�̃I�u�W�F�N�g�Ɠ����ꍇ
				m_pCur[nPriority]->m_pPrev = m_pPrev;		// ���݂̑O�̃I�u�W�F�N�g�ݒ�
			}

			m_pNext->m_pPrev = m_pPrev;						// ���̑O�̃I�u�W�F�N�g�ݒ�
		}

		if (m_pTop[nPriority] == this)
		{// �擪�̃I�u�W�F�N�g�������Ɠ����ꍇ
			m_pTop[nPriority] = m_pNext;					// �擪�̃I�u�W�F�N�g�����ɍX�V
		}
		if (m_pCur[nPriority] == this)
		{// ���݂̃I�u�W�F�N�g�������Ɠ����ꍇ
			m_pCur[nPriority] = m_pPrev;					// ���݂̃I�u�W�F�N�g��O�ɍX�V
		}

		m_pPrev = NULL;										// �O�̃I�u�W�F�N�g
		m_pNext = NULL;										// ���̃I�u�W�F�N�g


		delete this;										// �j��
		m_nNumPri[nPriority]--;								// �v���C�I���e�B���������炷
		m_nNumAll--;										// ���������炷
	}
}
