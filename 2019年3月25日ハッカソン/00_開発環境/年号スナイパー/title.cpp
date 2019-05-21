//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "title.h"			// �^�C�g��
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "fade.h"			// �t�F�[�h
#include "camera.h"			// �J����
#include "block.h"			// �u���b�N
#include "3dobject.h"		// 3D�w�i�I�u�W�F�N�g
#include "UI.h"				// ui
#include "floor.h"			// ��
#include "wall.h"			// ��
#include "effect.h"			// �G�t�F�N�g
#include "sound.h"			// �T�E���h

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

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{
	for (int nCountb = 0; nCountb < OBJTYPE_MAX; nCountb++)
	{//	�e�L�X�g�̃I�u�W�F�N�g�̎��
		m_textState[nCountb].nCnter = NULL;
		for (int nCount = 0; nCount < OBJ_MAX; nCount++)
		{//	�e�L�X�g�̒��g�̏�����
			m_textState[nCountb].nType[nCount] = NULL;		//	���
			m_textState[nCountb].nColType[nCount] = NULL;	//	�����蔻��̎��
			m_textState[nCountb].pos[nCount] = {};			//	�ʒu
			m_textState[nCountb].rot[nCount] = {};			//	��]
			m_textState[nCountb].size[nCount] = {};			//	�傫��
			m_textState[nCountb].pVtxFloor[nCount] = {};	//	���̒��_�̍���
			m_textState[nCountb].col[nCount] = {};			//	�F
			m_textState[nCountb].uv[nCount] = {};			//	UV
			m_textState[nCountb].nColType[nCount] = {};		//	�����蔻��̎��
			m_textState[nCountb].fNoneSpeed[nCount] = {};	//	�I�u�W�F�N�g�̒ʏ�̑���
			m_textState[nCountb].fFomSpeed[nCount] = {};	//	�I�u�W�F�N�g�̒Ǐ]����Ƃ��̑���
		}
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init(void)
{
	//	�A�e�L�X�g�ǂݍ���
	ObjectLoad();
	//	�G�t�F�N�g�̃e�L�X�g�̖��O�̓ǂݍ���
	CEffect::LoadNameEffect();	//	�G�t�F�N�g�̃e�L�X�g�ǂݍ���
	CEffect::LoadDataEffect();	//	�G�t�F�N�g�̃f�[�^�ǂݍ���
	CEffect::Load();			//	�G�t�F�N�g�̓ǂݍ���
	CUI::Load(CManager::MODE_TITLE);	//  Ui�̓ǂݍ���
	CBlock::Load();				//	�u���b�N�̓ǂݍ���
	C3Dobject::Load();			//	3D�I�u�W�F�N�g�̓ǂݍ���
	CFloor::Load();				//	���̓ǂݍ���
	CWall::Load();				//	�ǂ̓ǂݍ���

//	UI�̐���
	CUI::CreateMode(CManager::MODE_TITLE);
	for (int nCount = 0; nCount < m_textState[OBJTYPE_BLOCK].nCnter; nCount++)
	{//	�u���b�N�̐ݒ�@�ʒu�@��]�@�傫���@��ށ@�@�����蔻��̎��
		CBlock::Create
		(m_textState[OBJTYPE_BLOCK].pos[nCount], D3DXVECTOR3(m_textState[OBJTYPE_BLOCK].rot[nCount].x, D3DX_PI*m_textState[OBJTYPE_BLOCK].rot[nCount].y, m_textState[OBJTYPE_BLOCK].rot[nCount].z),
			m_textState[OBJTYPE_BLOCK].size[nCount], (CBlock::MODELTYPE)m_textState[OBJTYPE_BLOCK].nType[nCount], (CBlock::COLTYPE)m_textState[OBJTYPE_BLOCK].nColType[nCount]);
	}
	for (int nCount = 0; nCount < m_textState[OBJTYPE_3DPORI].nCnter; nCount++)
	{//	3D�|���S���̐ݒ�	�ʒu�@��]�@�傫���@�F�@UV�@���
		C3Dobject::Create
		(m_textState[OBJTYPE_3DPORI].pos[nCount], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_textState[OBJTYPE_3DPORI].size[nCount],
			m_textState[OBJTYPE_3DPORI].col[nCount], m_textState[OBJTYPE_3DPORI].uv[nCount], m_textState[OBJTYPE_3DPORI].nType[nCount]);
	}
	for (int nCount = 0; nCount < m_textState[OBJTYPE_FLOOR].nCnter; nCount++)
	{//	���ݒ�	�ʒu�@��]�@�傫���@���_�̍����@�F�@UV�@���	����
		CFloor::Create
		(m_textState[OBJTYPE_FLOOR].pos[nCount], D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_textState[OBJTYPE_FLOOR].size[nCount],
			m_textState[OBJTYPE_FLOOR].col[nCount], m_textState[OBJTYPE_FLOOR].uv[nCount], m_textState[OBJTYPE_FLOOR].nType[nCount], (CFloor::COLTYPE)m_textState[OBJTYPE_FLOOR].nColType[nCount]);
	}
	for (int nCount = 0; nCount < m_textState[OBJTYPE_WALL].nCnter; nCount++)
	{//	�ǐݒ�	�ʒu�@��]�@�傫���@�F�@UV�@���
		CWall::Create
		(m_textState[OBJTYPE_WALL].pos[nCount], D3DXVECTOR3(0.0f, D3DX_PI*m_textState[OBJTYPE_WALL].rot[nCount].y, 0.0f), m_textState[OBJTYPE_WALL].size[nCount],
			m_textState[OBJTYPE_WALL].col[nCount], m_textState[OBJTYPE_WALL].uv[nCount], m_textState[OBJTYPE_WALL].nType[nCount]);
	}

	m_nFadeCnt = 0;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTitle::Uninit(void)
{
	CUI::Unload();				// Ui�̓ǂݍ���

	CBlock::Unload();			//	�u���b�N�̊J��
	C3Dobject::Unload();		//	3D�|���S���̊J��
	CFloor::Unload();			//	���̊J��
	CUI::Unload();				//  Ui�̊J��
	CWall::Unload();			//	�ǂ̔j��
	CEffect::Unload();			//	�G�t�F�N�g�̊J��
	CScene::ReleaseMode();		//	���[�h�I��
}

//=============================================================================
// �X�V����
//=============================================================================
void CTitle::Update(void)
{
	CSound *pSound = CManager::GetSound();			//	�T�E���h�̎擾
	if (CFade::GetFade() == CFade::FADE_NONE)
	{// �t�F�[�h���Ă��Ȃ����
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
			CManager::GetInputXPad()->GetTrigger(XINPUT_GAMEPAD_A, 0) == true)
		{// �L�[�{�[�h�iENTER�j����͂�����
			CFade::Create(CManager::MODE_TUTORIAL, CFade::FADE_OUT, 3);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_PRESS);	//	���艹
		}
	}

	m_nFadeCnt++;
	if (m_nFadeCnt >= 500)
	{
		CFade::Create(CManager::MODE_RANKING, CFade::FADE_OUT, 3);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{

}

//=============================================================================
// �e�L�X�g�ǂݍ���
//=============================================================================
void CTitle::ObjectLoad(void)
{
	FILE *pFile;
	char read[128];										//	�ǂݍ��ޕ�����
	char *txstName[OBJTYPE_MAX][128] = {};	//	�ǂݍ��ރe�L�X�g�̃t�@�C��

	txstName[OBJTYPE_ITEM][0] = "data/TEXT/TITLE_MAP/ItemSet.txt";			//	�X�e�[�W1�̃A�C�e��
	txstName[OBJTYPE_BLOCK][0] = "data/TEXT/TITLE_MAP/BlockSet.txt";		//	�X�e�[�W1�u���b�N
	txstName[OBJTYPE_3DPORI][0] = "data/TEXT/TITLE_MAP/3DPorSet.txt";		//	�X�e�[�W1D�|���S���r���{�[�h
	txstName[OBJTYPE_FLOOR][0] = "data/TEXT/TITLE_MAP/FloorSet.txt";		//	�X�e�[�W1D�|���S����
	txstName[OBJTYPE_WALL][0] = "data/TEXT/TITLE_MAP/WallSet.txt";			//	�X�e�[�W1D�|���S����
	txstName[OBJTYPE_ENEMY][0] = "data/TEXT/TITLE_MAP/EnemySet.txt";		//	�X�e�[�W1D�|���S���G


	//-------------------------------------�A�C�e��--------------------------------------------
																			//	�ǂݍ��ރe�L�X�g
	pFile = fopen(txstName[OBJTYPE_ITEM][0], "r");
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &read[0]);
		while (read[0] != NULL && strcmp(&read[0], "ITEM_SET") == 0)
		{//	�����񂪓ǂݍ��܂�Ă����ꍇ
			do
			{
				fscanf(pFile, "%s", &read[0]);
				if (strcmp(&read[0], "POS") == 0)
				{//	�ʒu�̐ݒ�
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_ITEM].pos[m_textState[OBJTYPE_ITEM].nCnter].x, &m_textState[OBJTYPE_ITEM].pos[m_textState[OBJTYPE_ITEM].nCnter].y, &m_textState[OBJTYPE_ITEM].pos[m_textState[OBJTYPE_ITEM].nCnter].z);
				}
				else if (strcmp(&read[0], "SIZE") == 0)
				{//	�傫���̐ݒ�
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_ITEM].size[m_textState[OBJTYPE_ITEM].nCnter].x, &m_textState[OBJTYPE_ITEM].size[m_textState[OBJTYPE_ITEM].nCnter].y, &m_textState[OBJTYPE_ITEM].size[m_textState[OBJTYPE_ITEM].nCnter].z);
				}
				else if (strcmp(&read[0], "TYPE") == 0)
				{//	��ނ̐ݒ�
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_ITEM].nType[m_textState[OBJTYPE_ITEM].nCnter]);
				}
				else if (strcmp(&read[0], "COL") == 0)
				{//	�傫���̐ݒ�4
					fscanf(pFile, "%s%f%f%f%f",
						&read[0], &m_textState[OBJTYPE_ITEM].col[m_textState[OBJTYPE_ITEM].nCnter].r, &m_textState[OBJTYPE_ITEM].col[m_textState[OBJTYPE_ITEM].nCnter].g, &m_textState[OBJTYPE_ITEM].col[m_textState[OBJTYPE_ITEM].nCnter].b, &m_textState[OBJTYPE_ITEM].col[m_textState[OBJTYPE_ITEM].nCnter].a);
				}
				else if (strcmp(&read[0], "UV") == 0)
				{//	�傫���̐ݒ�4
					fscanf(pFile, "%s%f%f",
						&read[0], &m_textState[OBJTYPE_ITEM].uv[m_textState[OBJTYPE_ITEM].nCnter].x, &m_textState[OBJTYPE_ITEM].uv[m_textState[OBJTYPE_ITEM].nCnter].y);
				}
			} while (strcmp(&read[0], "ITEM_END") != 0);
			//	�ǂݍ��񂾃A�C�e���̐����Z
			m_textState[OBJTYPE_ITEM].nCnter++;
			//	���̕�����ǂݍ���
			fscanf(pFile, "%s", &read[0]);
		}
		//	�e�L�X�g���߂�
		fclose(pFile);
	}
	//-------------------------------------�u���b�N-------------------------------------------
	//	�ǂݍ��ރe�L�X�g
	pFile = fopen(txstName[OBJTYPE_BLOCK][0], "r");
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &read[0]);
		while (read[0] != NULL && strcmp(&read[0], "BLOCK_SET") == 0)
		{//	�����񂪓ǂݍ��܂�Ă����ꍇ
			do
			{
				fscanf(pFile, "%s", &read[0]);
				if (strcmp(&read[0], "POS") == 0)
				{//	�ʒu�̐ݒ�
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_BLOCK].pos[m_textState[OBJTYPE_BLOCK].nCnter].x, &m_textState[OBJTYPE_BLOCK].pos[m_textState[OBJTYPE_BLOCK].nCnter].y, &m_textState[OBJTYPE_BLOCK].pos[m_textState[OBJTYPE_BLOCK].nCnter].z);
				}
				else if (strcmp(&read[0], "ROT") == 0)
				{//	��]�̐ݒ�
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_BLOCK].rot[m_textState[OBJTYPE_BLOCK].nCnter].x, &m_textState[OBJTYPE_BLOCK].rot[m_textState[OBJTYPE_BLOCK].nCnter].y, &m_textState[OBJTYPE_BLOCK].rot[m_textState[OBJTYPE_BLOCK].nCnter].z);
				}
				else if (strcmp(&read[0], "SIZE") == 0)
				{//	�傫���̐ݒ�
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_BLOCK].size[m_textState[OBJTYPE_BLOCK].nCnter].x, &m_textState[OBJTYPE_BLOCK].size[m_textState[OBJTYPE_BLOCK].nCnter].y, &m_textState[OBJTYPE_BLOCK].size[m_textState[OBJTYPE_BLOCK].nCnter].z);
				}
				else if (strcmp(&read[0], "MODELTYPE") == 0)
				{//	��ނ̐ݒ�
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_BLOCK].nType[m_textState[OBJTYPE_BLOCK].nCnter]);
				}
				else if (strcmp(&read[0], "COLTYPE") == 0)
				{//	��ނ̐ݒ�
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_BLOCK].nColType[m_textState[OBJTYPE_BLOCK].nCnter]);
				}
			} while (strcmp(&read[0], "BLOCK_END") != 0);
			//	�ǂݍ��񂾃��f���̐����Z
			m_textState[OBJTYPE_BLOCK].nCnter++;
			//	���̕�����ǂݍ���
			fscanf(pFile, "%s", &read[0]);
		}
		fclose(pFile);
	}
	//-------------------------------------3D�|���S��-------------------------------------------
	//	�ǂݍ��ރe�L�X�g
	pFile = fopen(txstName[OBJTYPE_3DPORI][0], "r");
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &read[0]);
		while (read[0] != NULL && strcmp(&read[0], "3DPORIGON_SET") == 0)
		{//	�����񂪓ǂݍ��܂�Ă����ꍇ
			do
			{
				fscanf(pFile, "%s", &read[0]);
				if (strcmp(&read[0], "POS") == 0)
				{//	�ʒu�̐ݒ�
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_3DPORI].pos[m_textState[OBJTYPE_3DPORI].nCnter].x, &m_textState[OBJTYPE_3DPORI].pos[m_textState[OBJTYPE_3DPORI].nCnter].y, &m_textState[OBJTYPE_3DPORI].pos[m_textState[OBJTYPE_3DPORI].nCnter].z);
				}
				else if (strcmp(&read[0], "SIZE") == 0)
				{//	�傫���̐ݒ�4
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_3DPORI].size[m_textState[OBJTYPE_3DPORI].nCnter].x, &m_textState[OBJTYPE_3DPORI].size[m_textState[OBJTYPE_3DPORI].nCnter].y, &m_textState[OBJTYPE_3DPORI].size[m_textState[OBJTYPE_3DPORI].nCnter].z);
				}
				else if (strcmp(&read[0], "TYPE") == 0)
				{//	��ނ̐ݒ�
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_3DPORI].nType[m_textState[OBJTYPE_3DPORI].nCnter]);
				}
				else if (strcmp(&read[0], "COL") == 0)
				{//	�傫���̐ݒ�4
					fscanf(pFile, "%s%f%f%f%f",
						&read[0], &m_textState[OBJTYPE_3DPORI].col[m_textState[OBJTYPE_3DPORI].nCnter].r, &m_textState[OBJTYPE_3DPORI].col[m_textState[OBJTYPE_3DPORI].nCnter].g, &m_textState[OBJTYPE_3DPORI].col[m_textState[OBJTYPE_3DPORI].nCnter].b, &m_textState[OBJTYPE_3DPORI].col[m_textState[OBJTYPE_3DPORI].nCnter].a);
				}
				else if (strcmp(&read[0], "UV") == 0)
				{//	�傫���̐ݒ�4
					fscanf(pFile, "%s%f%f",
						&read[0], &m_textState[OBJTYPE_3DPORI].uv[m_textState[OBJTYPE_3DPORI].nCnter].x, &m_textState[OBJTYPE_3DPORI].uv[m_textState[OBJTYPE_3DPORI].nCnter].y);
				}
			} while (strcmp(&read[0], "3DPORIGON_END") != 0);
			//	�ǂݍ��񂾃��f���̐����Z
			m_textState[OBJTYPE_3DPORI].nCnter++;
			//	���̕�����ǂݍ���
			fscanf(pFile, "%s", &read[0]);
		}
		fclose(pFile);
	}
	//-------------------------------------��-------------------------------------------
	//�ǂݍ��ރe�L�X�g
	pFile = fopen(txstName[OBJTYPE_FLOOR][0], "r");
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &read[0]);
		while (read[0] != NULL && strcmp(&read[0], "FLOOR_SET") == 0)
		{//	�����񂪓ǂݍ��܂�Ă����ꍇ
			do
			{
				fscanf(pFile, "%s", &read[0]);
				if (strcmp(&read[0], "POS") == 0)
				{//	�ʒu�̐ݒ�
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_FLOOR].pos[m_textState[OBJTYPE_FLOOR].nCnter].x, &m_textState[OBJTYPE_FLOOR].pos[m_textState[OBJTYPE_FLOOR].nCnter].y, &m_textState[OBJTYPE_FLOOR].pos[m_textState[OBJTYPE_FLOOR].nCnter].z);
				}
				else if (strcmp(&read[0], "SIZE") == 0)
				{//	�傫���̐ݒ�
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_FLOOR].size[m_textState[OBJTYPE_FLOOR].nCnter].x, &m_textState[OBJTYPE_FLOOR].size[m_textState[OBJTYPE_FLOOR].nCnter].y, &m_textState[OBJTYPE_FLOOR].size[m_textState[OBJTYPE_FLOOR].nCnter].z);
				}
				else if (strcmp(&read[0], "COL") == 0)
				{//	�傫���̐ݒ�4
					fscanf(pFile, "%s%f%f%f%f",
						&read[0], &m_textState[OBJTYPE_FLOOR].col[m_textState[OBJTYPE_FLOOR].nCnter].r, &m_textState[OBJTYPE_FLOOR].col[m_textState[OBJTYPE_FLOOR].nCnter].g, &m_textState[OBJTYPE_FLOOR].col[m_textState[OBJTYPE_FLOOR].nCnter].b, &m_textState[OBJTYPE_FLOOR].col[m_textState[OBJTYPE_FLOOR].nCnter].a);
				}
				else if (strcmp(&read[0], "UV") == 0)
				{//	�傫���̐ݒ�4
					fscanf(pFile, "%s%f%f",
						&read[0], &m_textState[OBJTYPE_FLOOR].uv[m_textState[OBJTYPE_FLOOR].nCnter].x, &m_textState[OBJTYPE_FLOOR].uv[m_textState[OBJTYPE_FLOOR].nCnter].y);
				}
				else if (strcmp(&read[0], "TYPE") == 0)
				{//	��ނ̐ݒ�
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_FLOOR].nType[m_textState[OBJTYPE_FLOOR].nCnter]);
				}
				else if (strcmp(&read[0], "COLTYPE") == 0)
				{//	��ނ̐ݒ�
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_FLOOR].nColType[m_textState[OBJTYPE_FLOOR].nCnter]);
				}
			} while (strcmp(&read[0], "FLOOR_END") != 0);
			//	�ǂݍ��񂾃��f���̐����Z
			m_textState[OBJTYPE_FLOOR].nCnter++;
			//	���̕�����ǂݍ���
			fscanf(pFile, "%s", &read[0]);
		}
		fclose(pFile);
	}
	//-------------------------------------��-------------------------------------------
	//	�ǂݍ��ރe�L�X�g
	pFile = fopen(txstName[OBJTYPE_WALL][0], "r");
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &read[0]);
		while (read[0] != NULL && strcmp(&read[0], "WALL_SET") == 0)
		{//	�����񂪓ǂݍ��܂�Ă����ꍇ
			do
			{
				fscanf(pFile, "%s", &read[0]);
				if (strcmp(&read[0], "POS") == 0)
				{//	�ʒu�̐ݒ�
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_WALL].pos[m_textState[OBJTYPE_WALL].nCnter].x, &m_textState[OBJTYPE_WALL].pos[m_textState[OBJTYPE_WALL].nCnter].y, &m_textState[OBJTYPE_WALL].pos[m_textState[OBJTYPE_WALL].nCnter].z);
				}
				else if (strcmp(&read[0], "ROT_Y") == 0)
				{//	Y��]�̐ݒ�4
					fscanf(pFile, "%s%f",
						&read[0], &m_textState[OBJTYPE_WALL].rot[m_textState[OBJTYPE_WALL].nCnter].y);
				}
				else if (strcmp(&read[0], "SIZE") == 0)
				{//	�傫���̐ݒ�
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_WALL].size[m_textState[OBJTYPE_WALL].nCnter].x, &m_textState[OBJTYPE_WALL].size[m_textState[OBJTYPE_WALL].nCnter].y, &m_textState[OBJTYPE_WALL].size[m_textState[OBJTYPE_WALL].nCnter].z);
				}
				else if (strcmp(&read[0], "COL") == 0)
				{//	�傫���̐ݒ�4
					fscanf(pFile, "%s%f%f%f%f",
						&read[0], &m_textState[OBJTYPE_WALL].col[m_textState[OBJTYPE_WALL].nCnter].r, &m_textState[OBJTYPE_WALL].col[m_textState[OBJTYPE_WALL].nCnter].g, &m_textState[OBJTYPE_WALL].col[m_textState[OBJTYPE_WALL].nCnter].b, &m_textState[OBJTYPE_WALL].col[m_textState[OBJTYPE_WALL].nCnter].a);
				}
				else if (strcmp(&read[0], "UV") == 0)
				{//	�傫���̐ݒ�4
					fscanf(pFile, "%s%f%f",
						&read[0], &m_textState[OBJTYPE_WALL].uv[m_textState[OBJTYPE_WALL].nCnter].x, &m_textState[OBJTYPE_WALL].uv[m_textState[OBJTYPE_WALL].nCnter].y);
				}
				else if (strcmp(&read[0], "TYPE") == 0)
				{//	��ނ̐ݒ�
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_WALL].nType[m_textState[OBJTYPE_WALL].nCnter]);
				}


			} while (strcmp(&read[0], "WALL_END") != 0);
			//	�ǂݍ��񂾃��f���̐����Z
			m_textState[OBJTYPE_WALL].nCnter++;
			//	���̕�����ǂݍ���
			fscanf(pFile, "%s", &read[0]);
		}
		fclose(pFile);
	}

	//-------------------------------------�G-------------------------------------------
	//	�ǂݍ��ރe�L�X�g
	pFile = fopen(txstName[OBJTYPE_ENEMY][0], "r");
	if (pFile != NULL)
	{
		fscanf(pFile, "%s", &read[0]);
		while (read[0] != NULL && strcmp(&read[0], "ENEMY_SET") == 0)
		{//	�����񂪓ǂݍ��܂�Ă����ꍇ
			do
			{
				fscanf(pFile, "%s", &read[0]);
				if (strcmp(&read[0], "POS") == 0)
				{//	�ʒu�̐ݒ�
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_ENEMY].pos[m_textState[OBJTYPE_ENEMY].nCnter].x, &m_textState[OBJTYPE_ENEMY].pos[m_textState[OBJTYPE_ENEMY].nCnter].y, &m_textState[OBJTYPE_ENEMY].pos[m_textState[OBJTYPE_ENEMY].nCnter].z);
				}
				else if (strcmp(&read[0], "ROT") == 0)
				{//	�����̐ݒ�
					fscanf(pFile, "%s%f%f%f",
						&read[0], &m_textState[OBJTYPE_ENEMY].rot[m_textState[OBJTYPE_ENEMY].nCnter].x, &m_textState[OBJTYPE_ENEMY].rot[m_textState[OBJTYPE_ENEMY].nCnter].y, &m_textState[OBJTYPE_ENEMY].rot[m_textState[OBJTYPE_ENEMY].nCnter].z);
				}
				else if (strcmp(&read[0], "LIFE") == 0)
				{//	�̗͂̐ݒ�
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_ENEMY].nLife[m_textState[OBJTYPE_ENEMY].nCnter]);
				}
				else if (strcmp(&read[0], "TYPE") == 0)
				{//	��ނ̐ݒ�
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_ENEMY].nType[m_textState[OBJTYPE_ENEMY].nCnter]);
				}
				else if (strcmp(&read[0], "MOVELENGTH") == 0)
				{//	���������̐ݒ�
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_ENEMY].nMoveLength[m_textState[OBJTYPE_ENEMY].nCnter]);
				}
				else if (strcmp(&read[0], "NONESPEED") == 0)
				{//	�ʏ�̑���
					fscanf(pFile, "%s%f",
						&read[0], &m_textState[OBJTYPE_ENEMY].fNoneSpeed[m_textState[OBJTYPE_ENEMY].nCnter]);
				}
				else if (strcmp(&read[0], "FOMSPEED") == 0)
				{//	�Ǐ]����Ƃ��̑���
					fscanf(pFile, "%s%f",
						&read[0], &m_textState[OBJTYPE_ENEMY].fFomSpeed[m_textState[OBJTYPE_ENEMY].nCnter]);
				}
				else if (strcmp(&read[0], "AITYPE") == 0)
				{//	AI�̎�ނ̐ݒ�
					fscanf(pFile, "%s%d",
						&read[0], &m_textState[OBJTYPE_ENEMY].nAIType[m_textState[OBJTYPE_ENEMY].nCnter]);
				}

			} while (strcmp(&read[0], "ENEMY_END") != 0);
			//	�ǂݍ��񂾃��f���̐����Z
			m_textState[OBJTYPE_ENEMY].nCnter++;
			//	���̕�����ǂݍ���
			fscanf(pFile, "%s", &read[0]);
		}
		fclose(pFile);
	}
}

