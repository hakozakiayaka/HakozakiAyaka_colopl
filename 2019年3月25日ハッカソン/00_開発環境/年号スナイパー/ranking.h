//���@���@���@���@���@���@���@���@���@���@���@
//�����L���O���� [ranking.h]
//Author : Kobayashi_Sho-hei/���� ����
//���@���@���@���@���@���@���@���@���@���@���@
#ifndef _RANKING_H_
#define _RANKING_H_
#include "main.h"
#include "manager.h"
#include "game.h"

//==============================================
//�O���錾
//==============================================
class CDebug;
class CSound;
class CLight;
class CCamera;
class CScene2D;
class CTexture2D;
class CScene3D;
class CMeshField;
class CNumber;
class CScore;
class CModel;
class CPlayer;
class CEnemy;

//==============================================
//�h���Ȃ� �� �����L���O
//==============================================
class CRanking
{
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

	CRanking();
	~CRanking();

	static CRanking *Create(void);

	HRESULT Init();
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ObjectLoad(void);			// �I�u�W�F�N�g�̓ǂݍ���

	static CCamera *GetCamera(void) { return m_pCamera; }
	static CTexture2D *GetTexture2D(void) { return m_pTexture2D; }
	static CScene3D *GetScene3D(void) { return m_pScene3D; }
	static CMeshField *GetMeshField(void) { return m_pMeshField; }
	static CNumber *GetNumber(void) { return m_pNumber; }
	static CScore *GetScore(void) { return m_pScore; }
	static CPlayer *GetPlayer(void) { return m_pPlayer; }
	static CEnemy *GetEnemy(void) { return m_pEnemy; }


private:

	static CDebug *m_pDebug;
	static CSound *m_pSound;
	static CLight *m_pLight;
	static CCamera *m_pCamera;
	static CScene2D *m_pScene2D;
	static CTexture2D *m_pTexture2D;
	static CScene3D *m_pScene3D;
	static CMeshField *m_pMeshField;
	static CNumber *m_pNumber;
	static CScore *m_pScore;
	static CPlayer *m_pPlayer;
	static CEnemy *m_pEnemy;
	TEXTSTATE	    m_textState[OBJTYPE_MAX];	//	�e�L�X�g�ǂݍ��ޕϐ�[�ǂݍ��ރI�u�W�F�N�g�̎��]

	int m_nFadeCnt;//�����J�ڗp
};

#endif

