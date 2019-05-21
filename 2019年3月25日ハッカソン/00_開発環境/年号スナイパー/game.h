//=============================================================================
//
// �Q�[������ [game.h]
// Author :  YUTARO ABE
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "manager.h"	// �}�l�[�W���[

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define OBJ_MAX (2400)			//	�e�L�X�g�œǂݍ��߂�I�u�W�F�N�g�̍ő吔
#define MAX_FLOOR (3)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CFloor;
class CPause;
class CSound;
class CNumber;
class CTime;
class CPlayer;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGame
{// �Q�[��
public:
	typedef enum
	{
		GAMESTATE_NONE = 0,	//�������Ă��Ȃ�
		GAMESTATE0_NORMAL,	//�ʏ���
		GAMESTATE_END,		//�I�[�o�[���
		GAMESTATE_CLEAR,	//�N���A���
		GAMESTATE_MAX
	}GAMESTATE;

	typedef enum
	{//	�X�e�[�W�̎��
		STAGE_NONE = 0,		//	�����Ȃ�
		STAGE_001,			//	STAGE1
		STAGE_MAX
	}STAGETYPE;

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
		int				nLife[OBJ_MAX];			//	�I�u�W�F�N�g�����̐ݒ�
		int				nColType[OBJ_MAX];		//	�I�u�W�F�N�g����̐ݒ�
		int				nAIType[OBJ_MAX];		//	�I�u�W�F�N�gAI�̎��
		int				nMoveLength[OBJ_MAX];	//	�I�u�W�F�N�g����������
		float			fNoneSpeed[OBJ_MAX];	//	�I�u�W�F�N�g�̒ʏ�̑���
		float			fFomSpeed[OBJ_MAX];		//	�I�u�W�F�N�g�̒Ǐ]����Ƃ��̑���
		D3DXVECTOR3		size[OBJ_MAX];			//	�I�u�W�F�N�g�T�C�Y�̐ݒ�
		D3DXCOLOR		col[OBJ_MAX];			//	�I�u�W�F�N�g�̐F
		D3DXVECTOR2		uv[OBJ_MAX];			//	�I�u�W�F�N�g�̃e�N�X�`����UV
		D3DXVECTOR4		pVtxFloor[OBJ_MAX];		//	���̒��_�̈ʒu
	}TEXTSTATE;

	CGame();									//	�R���X�g���N�^
	~CGame();									//	�f�X�g���N�^
	HRESULT Init(STAGETYPE StageType,int nPlayerLife);							//	����������
	void Uninit(void);							//	�I������
	void Update(void);							//	�X�V����
	void Draw(void);							//	�`�揈��
	void ObjectLoad(STAGETYPE stage);			//	�I�u�W�F�N�g�̓ǂݍ���
	void SoundSE(int nNum);					//	���ʉ�
	static CFloor *GetFloor(int nNum) { return m_pFloor[nNum]; }
	static bool GetPause(void) { return m_bPause; }				//	�|�[�Y�̎擾
	static STAGETYPE GetStage(void) { return m_StageType; }		//	�Q�[���̃X�e�[�W�̎��
	static GAMESTATE GetGameState(void) { return m_GameState; }	//	�Q�[���̏�Ԃ̎擾
	static void SetGameState(GAMESTATE GameState) { m_GameState = GameState; }	//	�Q�[���X�e�[�g�̐ݒ�

	//�����L���O�֘A
	static CTime *GetTime(void) { return m_pTime; }

private:
	static CFloor  *m_pFloor[MAX_FLOOR];					//	��
	TEXTSTATE	    m_textState[OBJTYPE_MAX];				//	�e�L�X�g�ǂݍ��ޕϐ�[�ǂݍ��ރI�u�W�F�N�g�̎��]
	static CPause  *m_pPause;								//	Pause
	static bool     m_bPause;								//	Pausewp���Ă��邩�ǂ���
	static STAGETYPE m_StageType;							//	�X�e�[�W�̎��
	static GAMESTATE m_GameState;							//	�Q�[���̏��
	static int		m_nCounter;								//	�Q�[���̏�Ԃ̃J�E���^�[
	static CPlayer *m_pPlayer;								//�v���C���[�N���X�ɒl��Ԃ�
	int	m_nPlayerDispCut;									//	�v���C���[���Ȃ��Ȃ��Ă���̃J�E���g
	//�����L���O�֘A
	static CTime * m_pTime;

protected:

};

#endif