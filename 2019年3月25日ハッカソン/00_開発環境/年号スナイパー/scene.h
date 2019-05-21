//=============================================================================
//
// �V�[������ [scene.h]
// Author :  YUTARO ABE
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CScene
{// �V�[��
public:
	typedef enum
	{// �v���C�I���e�B
		PRIORITY_BG = 0,		// �w�i
		PRIORITY_1,				// 1
		PRIORITY_2,				// 2
		PRIORITY_3,				// 3
		PRIORITY_4,				// 4
		PRIORITY_5,				// 5w
		PRIORITY_PAUSE,			// �|�[�Y
		PRIORITY_UI,			// UI
		PRIORITY_FADE,			// �t�F�[�h
		PRIORITY_MAX
	}PRIORITY;

	typedef enum
	{// �I�u�W�F�N�g�^�C�v
		OBJTYPE_NONE = 0,		// �������Ă��Ȃ�
		OBJTYPE_MESHFIELD,		// ���b�V���t�B�[���h
		OBJTYPE_MESHWALL,		// ���b�V���E�H�[��
		OBJTYPE_WALL,			// ��
		OBJTYPE_BG,				// �w�i
		OBJTYPE_ENEMY,			// �G
		OBJTYPE_PLAYER,			// �v���C���[
		OBJTYPE_CYLINDER,		// �R���C�_�[
		OBJTYPE_BULLET2D,		// �e
		OBJTYPE_BULLET3D,		// �e
		OBJTYPE_EFFECT,			// �G�t�F�N�g
		OBJTYPE_ITEM,			// �A�C�e��
		OBJTYPE_BLOCK,			// �u���b�N
		OBJTEPE_FLOOR,			// ��
		OBJTYPE_3DOBJECT,		// 3D�I�u�W�F�N�g
		OBJECTE_ORBIT,			// �I�[�r�b�g
		OBJECTE_SHADOW,			// �e
		OBJTYPE_LIFENUMBER,
		OBJTYPE_NUMBER,			// ����
		OBJTYPE_UI,				//	UI
		OBJTYPE_HPBER3D,		//	HP�o�[
		OBJTYPE_SCORE,			// �X�R�A
		OBJTYPE_RANKING,		// �����L���O
		OBJTYPE_PAUSE,			// �|�[�Y
		OBJTYPE_TITLE,			// �^�C�g��
		OBJTYPE_FADE,			// �t�F�[�h
		OBJTYPE_CAMERA,			// �J����
		OBJTYPE_RESULT,			// ���U���g
		OBJTYPE_MAX
	}OBJTYPE;

	CScene(int nPriority, OBJTYPE objType);				// �R���X�g���N�^
	virtual ~CScene();									// �f�X�g���N�^

	static void ReleaseMode(void);						// �t�F�[�h�ȊO�j��
	static void ReleaseAll(void);						// �S�j��
	static void UpadteAll(void);						// �S�X�V
	static void DrawAll(void);							// �S�`��

	virtual HRESULT Init(void) = 0;						// ����������
	virtual void Uninit(void) = 0;						// �I������
	virtual void Update(void) = 0;						// �X�V����
	virtual void Draw(void) = 0;						// �`�揈��

	static int GetNumAll(void) { return m_nNumAll; }					// �����擾
	static int GetNumPri(int nPriority) { return m_nNumPri[nPriority]; }// �v���C�I���e�B�����擾

	void SetObjType(OBJTYPE objType)	{ m_objType = objType; }		// �I�u�W�F�N�g�^�C�v�ݒ�
	OBJTYPE GetObjType(void)			{ return m_objType; }			// �I�u�W�F�N�g�^�C�v�擾

	static void SetStartUp(int nStartUp) { m_nStartUp = nStartUp; }		// �X�V�ݒ�

	static CScene *GetScene(PRIORITY nPriority) { return m_pTop[nPriority]; }
	CScene *GetSceneNext(void) { return m_pNext; }

private:
	void Release(void);									// �J������

	static CScene *m_pTop[PRIORITY_MAX];				// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CScene *m_pCur[PRIORITY_MAX];				// ���݂̃I�u�W�F�N�g�ւ̃|�C���^

	CScene *m_pPrev;									// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CScene *m_pNext;									// ���̃I�u�W�F�N�g�ւ̃|�C���^

	int m_nPriority;									// ���
	OBJTYPE m_objType;									// ���
	bool m_bDeath;										// ���S�t���O

	static int m_nNumAll;								// ����
	static int m_nNumPri[PRIORITY_MAX];					// �v���C�I���e�B����

	static int m_nStartUp;								// �X�V�J�n�ԍ�

protected:
	void SetDeath(void) { m_bDeath = true; }			// �j������

};

#endif