//=============================================================================
//
// �Ǐ��� [wall.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene3d.h"		// �V�[��
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_WALL_TEX (3)

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CWall: public CScene3D
{// �V�[��3D�i�e�FCScene�j
public:

	CWall();															// �R���X�g���N�^
	~CWall();															// �f�X�g���N�^
																		// ����������
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType);
	void Uninit(void);													// �I������
	void Update(void);													// �X�V����
	void Draw(void);													// �`�揈��
																		// ����
	static CWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType);
	static HRESULT Load(void);											// �e�N�X�`���̓ǂݍ���
	static void Unload(void);											// �j��

																		// �����蔻��
	bool Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin);

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[MAX_WALL_TEX];					// �e�N�X�`�����ւ̃|�C���^

protected:

};

#endif