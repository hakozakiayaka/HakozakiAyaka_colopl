//=============================================================================
//
// ������ [floor.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _FLOOR_H_
#define _FLOOR_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"			// ���C��
#include "scene3d.h"		// �V�[��
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_FLOOR_TEX (6)

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CFloor : public CScene3D
{// �V�[��3D�i�e�FCScene�j
public:

	typedef enum
	{//	�u���b�N�̔���̎��
		COLTYPE_NONE = 0,	//	�����蔻��Ȃ�
		COLTYPE_BOX,		//	�{�b�N�X�R���W����
		COLTYPE_MAX
	}COLTYPE;

	CFloor();															// �R���X�g���N�^
	~CFloor();															// �f�X�g���N�^
																		// ����������
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV,int nType,COLTYPE coltype);
	void Uninit(void);													// �I������
	void Update(void);													// �X�V����
	void Draw(void);													// �`�揈��
																		// ����
	static CFloor *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType,COLTYPE coltype);
	static HRESULT Load(void);											// �e�N�X�`���̓ǂݍ���
	static void Unload(void);											// �j��
																		// �����蔻��
	bool Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 sizeMax, D3DXVECTOR3 sizeMin);
	bool GetHit(void) { return m_bHit; }								//	�����������̎擾
private:
	static LPDIRECT3DTEXTURE9		m_pTexture[MAX_FLOOR_TEX];			// �e�N�X�`�����ւ̃|�C���^
	bool							m_bHit;								// �������Ă��邩�ǂ���
	COLTYPE						m_colType;					// ���

protected:

};

#endif