//=============================================================================
//
// �^�C�g������ [title.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// �^�C�g��
#include "manager.h"	// �}�l�[�W���[
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CEnemy;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTitle
{// �^�C�g��
public:
	typedef enum
	{//	�I�u�W�F�N�g�̎��
		OBJTYPE_NONE = 0,	//	�m�[�}��
		OBJTYPE_ITEM,		//	�A�C�e��
		OBJTYPE_BLOCK,		//	�u���b�N
		OBJTYPE_3DPORI,		//	3D�|���S��
		OBJTYPE_FLOOR,		//	��
		OBJTYPE_WALL,		//	��
		OBJTYPE_ENEMY,		//	�G
		OBJTYPE_MAX			//	�ő吔
	}OBJECTTYPE;

	typedef struct
	{
		int				nCnter;					//	�I�u�W�F�N�g�̐��𐔂���
		D3DXVECTOR3		pos[OBJ_MAX];			//	�I�u�W�F�N�g�|�W�V�����̐ݒ�
		D3DXVECTOR3		rot[OBJ_MAX];			//	�I�u�W�F�N�g�|�W�V�����̐ݒ�
		int				nType[OBJ_MAX];			//	�I�u�W�F�N�g��ނ̐ݒ�
		int				nColType[OBJ_MAX];		//	�I�u�W�F�N�g����̐ݒ�
		D3DXVECTOR3		size[OBJ_MAX];			//	�I�u�W�F�N�g�T�C�Y�̐ݒ�
		D3DXCOLOR		col[OBJ_MAX];			//	�I�u�W�F�N�g�̐F
		D3DXVECTOR2		uv[OBJ_MAX];			//	�I�u�W�F�N�g�̃e�N�X�`����UV
		D3DXVECTOR4		pVtxFloor[OBJ_MAX];		//	���̒��_�̈ʒu
		int				nAIType[OBJ_MAX];		//	�I�u�W�F�N�gAI�̎��
		float			fNoneSpeed[OBJ_MAX];	//	�I�u�W�F�N�g�̒ʏ�̑���
		float			fFomSpeed[OBJ_MAX];		//	�I�u�W�F�N�g�̒Ǐ]����Ƃ��̑���
		int				nLife[OBJ_MAX];			//	�I�u�W�F�N�g�����̐ݒ�
		int				nMoveLength[OBJ_MAX];	//	�I�u�W�F�N�g����������
	}TEXTSTATE;

	CTitle();						// �R���X�g���N�^
	~CTitle();						// �f�X�g���N�^

	HRESULT Init(void);				// ����������
	void Uninit(void);				// �I������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��
	void ObjectLoad(void);			// �I�u�W�F�N�g�̓ǂݍ���

private:
	TEXTSTATE	    m_textState[OBJTYPE_MAX];	//	�e�L�X�g�ǂݍ��ޕϐ�[�ǂݍ��ރI�u�W�F�N�g�̎��]

	int m_nFadeCnt;//�����J�ڗp

protected:
};

#endif