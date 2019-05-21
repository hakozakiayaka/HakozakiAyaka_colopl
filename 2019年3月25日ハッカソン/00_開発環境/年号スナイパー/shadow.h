//=============================================================================
//
// �e���� [3dobject.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene3d.h"	// �V�[��
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CShadow : public CScene3D
{// �V�[��3D�i�e�FCScene�j
public:
	CShadow();															// �R���X�g���N�^
	~CShadow();															// �f�X�g���N�^
																		// ����������
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV);
	void Uninit(void);													// �I������
	void Update(void);													// �X�V����
	void Draw(void);													// �`�揈��
																		// ����
	static CShadow *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV);
	static HRESULT Load(void);											// �e�N�X�`���̓ǂݍ���
	static void Unload(void);											// �j��

private:
	static LPDIRECT3DTEXTURE9		m_pTexture;							// �e�N�X�`�����ւ̃|�C���^
	D3DXMATRIX						m_mtxWorld;							// ���[���h�}�g���b�N�X

protected:
};

#endif