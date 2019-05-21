//=============================================================================
//
// ������ [Floor.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene3D.h"		// �V�[��3D
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "floor.h"			// ��

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
LPDIRECT3DTEXTURE9 CFloor::m_pTexture[MAX_FLOOR_TEX] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CFloor::CFloor() : CScene3D(CScene::PRIORITY_2, CScene::OBJTEPE_FLOOR)
{
	m_bHit = false;			//	�����蔻��ɓ��������ǂ���
	m_colType = COLTYPE_NONE;										//	�����蔻��̎��
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFloor::~CFloor()
{
}

//=============================================================================
//	�A�C�e���̐���
//=============================================================================
CFloor *CFloor::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType, COLTYPE coltype)
{
	CFloor *pFloor = {};

	if (pFloor == NULL)
	{//	�A�C�e���̐���
		pFloor = new CFloor;
		if (pFloor != NULL)
		{//�A�C�e���̏�����
			pFloor->Init(pos, rot, size, col, TexUV, nType, coltype);
		}
	}
	return pFloor;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CFloor::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType, COLTYPE coltype)
{
	CScene3D::SetInitAll(pos + D3DXVECTOR3(0.0f, size.y, 0.0f), rot, size, col, TexUV, CScene3D::SCENE3DTYPE_NORMAL);
	CScene3D::Init();
	CScene3D::BindTexture(m_pTexture[nType]);

	m_colType = coltype;			//	���f���̔���̎��
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CFloor::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CFloor::Update(void)
{
	D3DXVECTOR3 FloorPos = CScene3D::Getpos();		//	�ǂ̈ʒu
	D3DXVECTOR3 FloorRot = CScene3D::Getrot();		//	�ǂ̉�]
	D3DXVECTOR3 FloorSize = CScene3D::Getsize();		//	�ǂ̕�
	CScene3D::Setpos(FloorPos);
	CScene3D::Setrot(FloorRot);
	CScene3D::Setsize(FloorSize);

	//CScene3D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CFloor::Draw(void)
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
HRESULT CFloor::Load(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\FLOOR\\floor000.jpg", &m_pTexture[0]);	//	��

	return S_OK;
}
//=============================================================================
// �A�����[�h����
//=============================================================================
void CFloor::Unload(void)
{
	for (int nCount = 0; nCount < MAX_FLOOR_TEX; nCount++)
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
//	���̓����蔻��
//=============================================================================
bool CFloor::Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin)
{
	m_bHit = false;								//	�����������������Ă��Ȃ���

	D3DXVECTOR3 FloorPos = CScene3D::Getpos();			//	���̈ʒu
	D3DXVECTOR3 FloorSize = CScene3D::Getsize();		//	�Ǐ��̕�

	if (m_colType == COLTYPE_BOX)
	{//	�R���W����BOX�������ꍇ
		if ((FloorPos.x - FloorSize.x) < (pos->x + (sizeMax.x)) &&
			(pos->x + (sizeMin.x)) < (FloorPos.x + FloorSize.x) &&
			(FloorPos.z - FloorSize.z) < (pos->z + (sizeMax.z)) &&
			(pos->z + (sizeMin.z)) < (FloorPos.z + FloorSize.z))
		{// X/Z�͈͊m�F
			if ((pos->y + sizeMin.y) <= (FloorPos.y + FloorSize.y) && (FloorPos.y + FloorSize.y) <= (posOld->y + sizeMin.y))
			{// �ォ��̓����蔻��
				m_bHit = true;
				pos->y = posOld->y;
				move->y = 0.0f;
			}
			else if ((FloorPos.y + FloorSize.y) <= (pos->y + sizeMax.y) && (posOld->y + sizeMax.y) <= (FloorPos.y + FloorSize.y))
			{// ������̓����蔻��
				m_bHit = true;
				pos->y = posOld->y;
				move->y = 0.0f;
			}
		}
		if ((pos->y + sizeMin.y) < (FloorPos.y + FloorSize.y) && (FloorPos.y - FloorSize.y) < (pos->y + sizeMax.y))
		{// Y�͈͊m�F
			if ((FloorPos.z - FloorSize.z) < (pos->z + (sizeMax.z)) && (pos->z + (sizeMin.z)) < (FloorPos.z + FloorSize.z))
			{// Z�͈͊m�F
				if ((FloorPos.x - FloorSize.x) <= (pos->x + sizeMax.x) && (posOld->x + sizeMax.x) <= (FloorPos.x - FloorSize.x))
				{// ������̓����蔻
					m_bHit = true;
					pos->x = posOld->x;
					move->x = 0;
				}
				else if ((pos->x + sizeMin.x) <= (FloorPos.x + FloorSize.x) && (FloorPos.x + FloorSize.x) <= (posOld->x + sizeMin.x))
				{// �E����̓����蔻��
					m_bHit = true;
					pos->x = posOld->x;
					move->x = 0;
				}
			}
			if ((FloorPos.x - FloorSize.x) < (pos->x + (sizeMax.x)) && (pos->x + (sizeMin.x)) < (FloorPos.x + FloorSize.x))
			{// X�͈͊m�F
				if ((FloorPos.z - FloorSize.z) <= (pos->z + sizeMax.z) && (posOld->z + sizeMax.z) <= (FloorPos.z - FloorSize.z))
				{// ��O����̓����蔻��
					m_bHit = true;
					pos->z = posOld->z;
					move->z = 0;
				}
				else if ((pos->z + sizeMin.z) <= (FloorPos.z + FloorSize.z) && (FloorPos.z + FloorSize.z) <= (posOld->z + sizeMin.z))
				{// ��납��̓����蔻��
					m_bHit = true;
					pos->z = posOld->z;
					move->z = 0;
				}
			}
		}
	}
	return m_bHit;
}