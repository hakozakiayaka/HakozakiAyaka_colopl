//=============================================================================
//
// ���C�g���� [light.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "light.h"			// ���C�g
#include "manager.h"		// �}�l�[�W���[
#include "input.h"			// ����
#include "renderer.h"		// �����_���[

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
// �R���X�g���N�^									(public)	*** CLight ***
//=============================================================================
CLight::CLight()
{

}

//=============================================================================
// �f�X�g���N�^										(public)	*** CLight ***
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
// ����������										(public)	*** CLight ***
//=============================================================================
HRESULT CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X�̎擾
	D3DXVECTOR3 vecDir[MAX_LIGHT];				// �ݒ�p�����x�N�g��

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ���C�g���N���A����
		ZeroMemory(&m_Light[nCntLight], sizeof(D3DLIGHT9));

		if (nCntLight == 0)
		{
			// ���C�g�̎�ނ�ݒ�
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ���C�g�̊g�U����ݒ�
			m_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���C�g�̕����̐ݒ�
			vecDir[nCntLight] = D3DXVECTOR3(-0.45f, -0.38f, 0.35f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// ���K������
			m_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 1)
		{
			// ���C�g�̎�ނ�ݒ�
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ���C�g�̊g�U����ݒ�
			m_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���C�g�̕����̐ݒ�
			vecDir[nCntLight] = D3DXVECTOR3(0.22f, -0.37f, 0.44f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// ���K������
			m_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 2)
		{
			// ���C�g�̎�ނ�ݒ�
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ���C�g�̊g�U����ݒ�
			m_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���C�g�̕����̐ݒ�
			vecDir[nCntLight] = D3DXVECTOR3(-0.22f, -0.37f, 0.44f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// ���K������
			m_Light[nCntLight].Direction = vecDir[nCntLight];
		}
		else if (nCntLight == 3)
		{
			// ���C�g�̎�ނ�ݒ�
			m_Light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// ���C�g�̊g�U����ݒ�
			m_Light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���C�g�̕����̐ݒ�
			vecDir[nCntLight] = D3DXVECTOR3(-0.22f, -0.27f, 0.44f);
			D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		// ���K������
			m_Light[nCntLight].Direction = vecDir[nCntLight];
		}

		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &m_Light[nCntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}

	return S_OK;
}

//=============================================================================
// �I������											(public)	*** CLight ***
//=============================================================================
void CLight::Uninit(void)
{

}

//=============================================================================
// �X�V����											(public)	*** CLight ***
//=============================================================================
void CLight::Update(void)
{

}