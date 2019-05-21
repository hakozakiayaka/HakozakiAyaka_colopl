//=============================================================================
//
// 3Dobject���� [3dobject.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _3DOBJECT_H_
#define _3DOBJECT_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene3d.h"		// �V�[��
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
class C3Dobject : public CScene3D
{// �V�[��3D�i�e�FCScene�j
public:
	typedef enum
	{
		TYPE_001 = 0,
		TYPE_002,
		TYPE_003,
		TYPE_004,
		TYPE_005,
		TYPE_006,
		TYPE_007,
		TYPE_008,
		TYPE_009,
		TYPE_MAX
	}OBJECTTYPE;

	C3Dobject();														// �R���X�g���N�^
	~C3Dobject();														// �f�X�g���N�^
																		// ����������
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType);
	void Uninit(void);													// �I������
	void Update(void);													// �X�V����
	void Draw(void);													// �`�揈��
																		// ����
	static C3Dobject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType);
	static HRESULT Load(void);											// �e�N�X�`���̓ǂݍ���
	static void Unload(void);											// �j��

private:
	static LPDIRECT3DTEXTURE9		m_pTexture[TYPE_MAX];					// �e�N�X�`�����ւ̃|�C���^
	OBJECTTYPE				m_Type;											// �A�C�e���̎��

protected:

};

#endif