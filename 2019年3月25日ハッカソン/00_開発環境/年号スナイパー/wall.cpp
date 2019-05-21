//=============================================================================
//
// �Ǐ��� [wall.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene3D.h"		// �V�[��3D
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "wall.h"

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
LPDIRECT3DTEXTURE9 CWall::m_pTexture[MAX_WALL_TEX] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CWall::CWall() : CScene3D(CScene::PRIORITY_3, CScene::OBJTYPE_WALL)
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CWall::~CWall()
{
}

//=============================================================================
//	�A�C�e���̐���
//=============================================================================
CWall *CWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType)
{
	CWall *pWall = {};

	if (pWall == NULL)
	{//	�A�C�e���̐���
		pWall = new CWall;
		if (pWall != NULL)
		{//�A�C�e���̏�����
			pWall->Init(pos, rot, size, col, TexUV, nType);
		}
	}
	return pWall;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CWall::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType)
{
	CScene3D::SetInitAll(pos + D3DXVECTOR3(0.0f, size.y, 0.0f), rot, size, col, TexUV, CScene3D::SCENE3DTYPE_NORMAL);
	CScene3D::Init();
	CScene3D::BindTexture(m_pTexture[nType]);
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CWall::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CWall::Update(void)
{
	D3DXVECTOR3 WallPos = CScene3D::Getpos();		//	�ʒu�̎擾
	D3DXVECTOR3 WallRot = CScene3D::Getrot();		//	��]�̎擾
	D3DXVECTOR3 WallSize = CScene3D::Getsize();		//	���̎擾

	CScene3D::Setpos(WallPos);						//	�ʒu�̐ݒ�
	CScene3D::Setrot(WallRot);						//	��]�̐ݒ�
	CScene3D::Setsize(WallSize);					//	�傫���̐ݒ�

	CScene3D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CWall::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// �J�����O�Ȃ���

	CScene3D::Draw();
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// ���ʂ��J�����O�ɖ߂�

}
//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CWall::Load(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, NULL, &m_pTexture[0]);		//	�ǃm�[�}��
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\WALL\\backBlock.jpg", &m_pTexture[1]);	//	�ǖ�
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\WALL\\wall000.jpg", &m_pTexture[2]);		//	�ǖ�

	return S_OK;
}
//=============================================================================
// �A�����[�h����
//=============================================================================
void CWall::Unload(void)
{
	for (int nCount = 0; nCount < MAX_WALL_TEX; nCount++)
	{
		// �e�N�X�`���̔j��
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}

//=============================================================================
//	�ǂ̓����蔻��
//=============================================================================
bool CWall::Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin)
{
	bool bHit = false;								//	�����������������Ă��Ȃ���

	D3DXVECTOR3 WallPos = CScene3D::Getpos();		//	�ʒu�̎擾
	D3DXVECTOR3 WallSize = CScene3D::Getsize();		//	���̎擾
	D3DXVECTOR3 WallRot = CScene3D::Getrot();		//	��]�̎擾
	if (WallRot.y == 0.0f || WallRot.y == D3DX_PI * 1.0f)
	{//	��O���������������ꍇ
		WallSize.x = WallSize.x;
		WallSize.z = WallSize.z;

		if ((WallPos.x - WallSize.x) < (pos->x + (sizeMax.x)) &&
			(pos->x + (sizeMin.x)) < (WallPos.x + WallSize.x) &&
			(WallPos.z - WallSize.z) < (pos->z + (sizeMax.z)) &&
			(pos->z + (sizeMin.z)) < (WallPos.z + WallSize.z))
		{// X/Z�͈͊m�F
			if ((pos->y + sizeMin.y) <= (WallPos.y + WallSize.y) && (WallPos.y + WallSize.y) <= (posOld->y + sizeMin.y))
			{// �ォ��̓����蔻��
				bHit = true;
				pos->y = posOld->y;
				move->y = 0.0f;
			}
			else if ((WallPos.y + WallSize.y) <= (pos->y + sizeMax.y) && (posOld->y + sizeMax.y) <= (WallPos.y + WallSize.y))
			{// ������̓����蔻��
				bHit = true;
				pos->y = posOld->y;
				move->y = 0.0f;
			}
		}
		if ((pos->y + sizeMin.y) < (WallPos.y + WallSize.y) && (WallPos.y - WallSize.y) < (pos->y + sizeMax.y))
		{// Y�͈͊m�F
			if ((WallPos.z - WallSize.z) < (pos->z + (sizeMax.z)) && (pos->z + (sizeMin.z)) < (WallPos.z + WallSize.z))
			{// Z�͈͊m�F
				if ((WallPos.x - WallSize.x) <= (pos->x + sizeMax.x) && (posOld->x + sizeMax.x) <= (WallPos.x - WallSize.x))
				{// ������̓����蔻
					bHit = true;
					pos->x = posOld->x;
					//move->z = 0;
					move->x = 0;
				}
				else if ((pos->x + sizeMin.x) <= (WallPos.x + WallSize.x) && (WallPos.x + WallSize.x) <= (posOld->x + sizeMin.x))
				{// �E����̓����蔻��
					bHit = true;
					pos->x = posOld->x;
					//move->z = 0;
					move->x = 0;
				}
			}
			if ((WallPos.x - WallSize.x) < (pos->x + (sizeMax.x)) && (pos->x + (sizeMin.x)) < (WallPos.x + WallSize.x))
			{// X�͈͊m�F
				if ((WallPos.z - WallSize.z) <= (pos->z + sizeMax.z) && (posOld->z + sizeMax.z) <= (WallPos.z - WallSize.z))
				{// ��O����̓����蔻��
					bHit = true;
					pos->z = posOld->z;
					move->z = 0;
					//move->x = 0;

				}
				else if ((pos->z + sizeMin.z) <= (WallPos.z + WallSize.z) && (WallPos.z + WallSize.z) <= (posOld->z + sizeMin.z))
				{// ��납��̓����蔻��
					bHit = true;
					pos->z = posOld->z;
					move->z = 0;
					//move->x = 0;
				}
			}
		}
	}
	else if (WallRot.y == D3DX_PI * 0.5f || WallRot.y == D3DX_PI * -0.5f)
	{//	�E�������������������ꍇ
		//WallSize.x = WallSize.z;
		//WallSize.z = WallSize.x;

		if ((WallPos.x - WallSize.z) < (pos->x + (sizeMax.x)) &&
			(pos->x + (sizeMin.x)) < (WallPos.x + WallSize.z) &&
			(WallPos.z - WallSize.x) < (pos->z + (sizeMax.z)) &&
			(pos->z + (sizeMin.z)) < (WallPos.z + WallSize.x))
		{// X/Z�͈͊m�F
			if ((pos->y + sizeMin.y) <= (WallPos.y + WallSize.y) && (WallPos.y + WallSize.y) <= (posOld->y + sizeMin.y))
			{// �ォ��̓����蔻��
				bHit = true;
				pos->y = posOld->y;
				move->y = 0.0f;
			}
			else if ((WallPos.y + WallSize.y) <= (pos->y + sizeMax.y) && (posOld->y + sizeMax.y) <= (WallPos.y + WallSize.y))
			{// ������̓����蔻��
				bHit = true;
				pos->y = posOld->y;
				move->z = 0;
				move->y = 0.0f;
			}
		}
		if ((pos->y + sizeMin.y) < (WallPos.y + WallSize.y) && (WallPos.y - WallSize.y) < (pos->y + sizeMax.y))
		{// Y�͈͊m�F
			if ((WallPos.z - WallSize.x) < (pos->z + (sizeMax.z)) && (pos->z + (sizeMin.z)) < (WallPos.z + WallSize.x))
			{// Z�͈͊m�F
				if ((WallPos.x - WallSize.z) <= (pos->x + sizeMax.x) && (posOld->x + sizeMax.x) <= (WallPos.x - WallSize.z))
				{// ������̓����蔻
					bHit = true;
					pos->x = posOld->x;
					//move->z = 0;
					move->x = 0;
				}
				else if ((pos->x + sizeMin.x) <= (WallPos.x + WallSize.z) && (WallPos.x + WallSize.z) <= (posOld->x + sizeMin.x))
				{// �E����̓����蔻��
					bHit = true;
					pos->x = posOld->x;
					move->x = 0;
				}
			}
			if ((WallPos.x - WallSize.z) < (pos->x + (sizeMax.x)) && (pos->x + (sizeMin.x)) < (WallPos.x + WallSize.z))
			{// X�͈͊m�F
				if ((WallPos.z - WallSize.z) <= (pos->z + sizeMax.z) && (posOld->z + sizeMax.z) <= (WallPos.z - WallSize.x))
				{// ��O����̓����蔻��
					bHit = true;
					pos->z = posOld->z;
					move->x = 0;
					//move->z = 0;
				}
				else if ((pos->z + sizeMin.z) <= (WallPos.z + WallSize.x) && (WallPos.z + WallSize.x) <= (posOld->z + sizeMin.z))
				{// ��납��̓����蔻��
					bHit = true;
					pos->z = posOld->z;
					move->z = 0;
					//move->x = 0;
				}
			}
		}
	}
	//	�l��Ԃ�
	return bHit;

}