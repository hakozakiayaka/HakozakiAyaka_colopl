//=============================================================================
//
// ���f������ [model.cpp]
// Author : �����ʉ�
//
//=============================================================================
// �t�@�C���ǂݍ��݂ŕK�v
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>				//�C���N���[�h�t�@�C��
#include <string.h>

#include "manager.h"			// �}�l�[�W���[
#include "renderer.h"			// �����_�����O
#include "input.h"				// ����
#include "scene.h"				// scene
#include "model.h"				// ���f��
#include "DebugProc.h"		// �f�o�b�O�\��

//*****************************************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CModel::m_pTexture = NULL;		// �e�N�X�`���̃|�C���^
char CModel::m_acFileName[MAX_MODEL][MAX_CHAR] = {};		// �ǂݍ��ރ��f���̖��O

															//=============================================================================
															// �R���X�g���N�^
															//=============================================================================
CModel::CModel()
{
	m_apMesh = NULL;
	m_apBuffMat = NULL;
	m_aNumMat = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CModel::~CModel()
{
}

#if 1
//=============================================================================
// �p�[�c�ǂݍ���
//=============================================================================
HRESULT CModel::Load(LPCSTR Memory)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;											//�f�o�C�X�̎擾
	CRenderer * pRenderer = CManager::GetRenderer();						//�����_�����O�N���X�̌Ăяo��

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(Memory, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_apBuffMat, NULL, &m_aNumMat, &m_apMesh);

	return S_OK;
}
#endif

//=============================================================================
// �p�[�c�̔j��
//=============================================================================
void CModel::Unload(void)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		m_acFileName[nCntModel][0] = NULL;
	}
}

//=============================================================================
// ���f���̐���
//=============================================================================
CModel * CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumParts, MODELTYPE type)
{
	CModel * pModel = NULL;

	if (pModel == NULL)
	{	// ���f���̓��I�m��
		pModel = new CModel;
	}
	if (pModel != NULL)
	{
		pModel->m_nNumParts = nNumParts;
		pModel->m_type = type;
		pModel->Init();				// ������
		pModel->m_pos = pos;	// �ʒu
		pModel->m_rot = rot;		// ����
	}

	return pModel;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CModel::Init(void)
{	// �l�̏������͂��܂��I�i�p�[�c���S�ď��������ꂿ�Ⴄ�j

	LPDIRECT3DDEVICE9 pDevice = NULL;									// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();						// �����_�����O�N���X�̌Ăяo��

	if (pRenderer != NULL)
	{	//Renderer�N���X�̎擾
		pDevice = pRenderer->GetDevice();
	}


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CModel::Uninit(void)
{

	// ���b�V���̊J��
	if (m_apMesh != NULL)
	{
		m_apMesh->Release();
		m_apMesh = NULL;
	}

	// �}�e���A���̊J��
	if (m_apBuffMat != NULL)
	{
		m_apBuffMat->Release();
		m_apBuffMat = NULL;
	}

	if (m_aNumMat != NULL)
	{
		m_aNumMat = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CModel::Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void CModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;										// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();						// �����_�����O�N���X�̌Ăяo��

	if (pRenderer != NULL)
	{	//Renderer�N���X�̎擾
		pDevice = pRenderer->GetDevice();
	}

	D3DXMATRIX mtxRot, mtxTrans, mtxParent;						//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;										//���݂̃}�g���b�N�X�ۑ��p
	D3DXMATERIAL * pMat;										//�}�e���A���f�[�^�ւ̃|�C���^

	D3DXMATRIX mtxShadow;										//�@�v�Z�p�}�g���b�N�X
	D3DXPLANE planeField;										// �v���[���t�B�[���h
	D3DXVECTOR4 vecLight;										//�@���C�g�̃x�N�g��
	D3DXVECTOR3 pos, normal;									// �ʒu�A�@��

																// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��i�ʒu�j�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// �e�̃}�g���b�N�X�Ɗ|�����킹��
	if (m_pParent != NULL)
	{
		mtxParent = m_pParent->GetMtxWorld();			// CModel�̃}�g���b�N�X���擾
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// �v���C���[�ƃ��f���̃}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	if (m_apMesh != NULL)
	{
		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)m_apBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_aNumMat; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			pDevice->SetTexture(0, m_pTexture);

			// ���f��(�p�[�c)�̕`��
			m_apMesh->DrawSubset(nCntMat);
		}
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �e�N�X�`���̊��蓖��
//=============================================================================
void CModel::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;
}