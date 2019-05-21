//=============================================================================
//
// �r���{�[�h�I�u�W�F�N�g���� [item.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene3D.h"		// �V�[��3D
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����
#include "camera.h"			// �J����
#include "3dobject.h"		// 3d�I�u�W�F�N�g
#include "effect.h"			// �G�t�F�N�g

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
LPDIRECT3DTEXTURE9 C3Dobject::m_pTexture[TYPE_MAX] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
C3Dobject::C3Dobject() : CScene3D(CScene::PRIORITY_5, CScene::OBJTYPE_3DOBJECT)
{
	m_Type = {};
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
C3Dobject::~C3Dobject()
{
}

//=============================================================================
//	�A�C�e���̐���
//=============================================================================
C3Dobject *C3Dobject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType)
{
	C3Dobject *p3Dobject = {};

	if (p3Dobject == NULL)
	{//	�A�C�e���̐���
		p3Dobject = new C3Dobject;
		if (p3Dobject != NULL)
		{//�A�C�e���̏�����
			p3Dobject->Init(pos, rot, size, col, TexUV, nType);
		}
	}
	return p3Dobject;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT C3Dobject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType)
{
	CScene3D::SetInitAll(pos + D3DXVECTOR3(0.0f, size.y,0.0f), rot, size, col, TexUV, CScene3D::SCENE3DTYPE_BILLBOARD);
	CScene3D::Init();
	CScene3D::BindTexture(m_pTexture[nType]);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void C3Dobject::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void C3Dobject::Update(void)
{
	CScene3D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void C3Dobject::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();			// �f�o�C�X�擾

	CScene3D::Draw();

}
//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT C3Dobject::Load(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\tree003.png", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\tree001.png", &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\tree002.png", &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\tree002.png", &m_pTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\kusa000.png", &m_pTexture[4]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\kusa001.png", &m_pTexture[5]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\iwa000.png", &m_pTexture[6]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\iwa001.png", &m_pTexture[7]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\bamboo001.png", &m_pTexture[8]);

	return S_OK;
}
//=============================================================================
// �A�����[�h����
//=============================================================================
void C3Dobject::Unload(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		// �e�N�X�`���̔j��
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}
