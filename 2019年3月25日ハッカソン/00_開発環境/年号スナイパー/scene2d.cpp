//=============================================================================
//
// �V�[��2D���� [scene2d.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "scene2d.h"		// �V�[��2D
#include "manager.h"		// �}�l�[�W���[
#include "renderer.h"		// �����_���[
#include "input.h"			// ����

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
CScene2D::CScene2D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// �����l�ݒ�
	m_pTexture	= NULL;									// �e�N�X�`�����ւ̃|�C���^
	m_pVtxBuff	= NULL;									// �}�e���A�����ւ̃|�C���^

	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_size		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �傫��
	m_col		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// �F
	m_TexUV		= D3DXVECTOR2(0.0f, 0.0f);				// UV
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScene2D::Init(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&m_pVtxBuff, NULL);

	VERTEX_2D *pVtx;	// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f) + m_pos;
	pVtx[1].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f) + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f) + m_pos;
	pVtx[3].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f) + m_pos;

	// 1.0f�ŌŒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// ���_�J���[�ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	//	�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_TexUV.x, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, m_TexUV.y);
	pVtx[3].tex = D3DXVECTOR2(m_TexUV.x, m_TexUV.y);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScene2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{// NULL�ȊO�̏ꍇ
		m_pVtxBuff->Release();		// ���
		m_pVtxBuff = NULL;			// NULL��
	}

	// �V�[���J��
	CScene::SetDeath();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScene2D::Update(void)
{
	VERTEX_2D *pVtx;	// ���_���ւ�

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f) + m_pos;
	pVtx[1].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f) + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f) + m_pos;
	pVtx[3].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f) + m_pos;

	// ���_�J���[�ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//	�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_TexUV.x, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, m_TexUV.y);
	pVtx[3].tex = D3DXVECTOR2(m_TexUV.x, m_TexUV.y);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene2D::Draw(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
 	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//=============================================================================
//	�e�N�X�`���̐ݒ�
//=============================================================================
void CScene2D::SetTexture(D3DXVECTOR2 *pTex)
{
	//���_���ւ̃|�C���^
	VERTEX_2D*pVtx;
	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W
	pVtx[0].tex = pTex[0];
	pVtx[1].tex = pTex[1];
	pVtx[2].tex = pTex[2];
	pVtx[3].tex = pTex[3];

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}