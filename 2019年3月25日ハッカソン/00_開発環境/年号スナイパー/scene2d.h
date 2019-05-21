//=============================================================================
//
// �V�[��2D���� [scene2d.h]
// Author :  YUTARO ABE
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene.h"		// �V�[��

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene2D : public CScene
{// �V�[��3D�i�e�FCScene�j
public:
	CScene2D(int nPriority, CScene::OBJTYPE objType);		// �R���X�g���N�^
	~CScene2D();								// �f�X�g���N�^

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }		// �e�N�X�`�����f

	HRESULT Init(void);							// ����������
	void Uninit(void);							// �I������
	void Update(void);							// �X�V����
	void Draw(void);							// �`�揈��

	void SetInitAll(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV)
	{
		m_pos = pos;
		m_size = size;
		m_col = col;
		m_TexUV = TexUV;
	}

	void Setpos(D3DXVECTOR3 pos) { m_pos = pos; }		// �ʒu�ݒ�
	D3DXVECTOR3 Getpos(void) { return m_pos; }			// �ʒu�擾
	void Setsize(D3DXVECTOR3 size) { m_size = size; }	// �傫���ݒ�
	D3DXVECTOR3 Getsize(void) { return m_size; }		// �傫���擾
	void Setcol(D3DXCOLOR col) { m_col = col; }			// �F�ݒ�
	D3DXCOLOR Getcol(void) { return m_col; }			// �F�擾

	void SetTexture(D3DXVECTOR2 *pTex);			//	�A�j���[�V�����̐ݒ�

private:
	LPDIRECT3DTEXTURE9		m_pTexture;			// �e�N�X�`�����ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			// ���_�o�b�t�@���ւ̃|�C���^

	D3DXVECTOR3 m_pos;							// �ʒu
	D3DXVECTOR3 m_size;							// �傫��
	D3DXCOLOR m_col;							// �F
	D3DXVECTOR2 m_TexUV;						// UV

protected:

};

#endif