//=============================================================================
//
// �v���C���[���� [player.h]
// Author : �����ʉ�
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

// �t�@�C���ǂݍ��݂ŕK�v
#include <stdio.h>			//�C���N���[�h�t�@�C��

#include "main.h"			// ���C��
#include "scene.h"			// scene

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_PRIORITY					(2)				// �v���C���[�̗D�揇��
#define MODEL_SPEED						(0.05f)			//���f���̈ړ��̑���
#define MAX_CHAR						(256)			// �����̍ő吔
#define MAX_KEY							(11)			// �L�[�̑���
#define JUMP							(10.0f)			// �W�����v
#define PLAYER_MOVE						(0.2f)			// ��̈ړ���
#define MAX_PLAYER_PARTS				(12)			//�v���C���[�̃p�[�c��
#define PLAYER_LIFE						(3)				// �v���C���[�̃��C�t
#define PLAYER_FILENAME			"data/TEXT/motion_souri.txt"	//�ǂݍ��ރt�@�C����

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CModel;		// ���f��

//*****************************************************************************
// �v���C���[�N���X
//*****************************************************************************
class CPlayer : public CScene
{
	//*****************************************************************************
	// �\���̂̒�`
	//*****************************************************************************
	typedef struct
	{	// �L�[�v�f
		D3DXVECTOR3 pos[MAX_PLAYER_PARTS];		//�ʒu���
		D3DXVECTOR3 rot[MAX_PLAYER_PARTS];		//�p�x���
		int nFrame;		// �t���[����
	} KEY;

	typedef struct
	{	// �L�[���
		int nLoop;		// ���[�v���邩���Ȃ���
		int nNumKey;	// �L�[��
		KEY aKey[MAX_KEY];
	} KEY_INFO;

public:
	typedef enum
	{	// �v���C���[�̏��
		PLAYERSTATE_NORMAL = 0,		// �ʏ���
		PLAYERSTATE_MOVE,					// �ړ����
		PLAYERSTATE_TURNUP,				// �U�����
		PLAYERSTATE_DEATH,					// �_���[�W���
		PLAYERSTATE_MAX						// �X�e�[�g�̍ő吔
	} PLAYERSTATE;

	typedef enum
	{	// ���[�V�����̎��
		MOTIONSTATE_NEUTRAL = 0,		// �j���[�g����
		MOTIONSTATE_MOVE,					// �ړ�
		MOTIONSTATE_TURNUP,				// �U��
		MOTIONSTATE_DEATH,					// �W�����v
		MOTIONSTATE_MAX						// �X�e�[�g�̍ő吔
	} MOTIONSTATE;

	typedef enum
	{	// �t���O�̎��
		PLAYERBOOLTYPE_JUMP = 0,		// �W�����v
		PLAYERBOOLTYPE_MOVE,			// �ړ�
		PLAYERBOOLTYPE_TURNUP,			// ���\
		PLAYERBOOLTYPE_DEATH,			// ����
		PLAYERBOOLTYPE_MAX				// �^�C�v�̍ő吔
	} PLAYERBOOLTYPE;

	CPlayer(int nPriority = 3, OBJTYPE type = OBJTYPE_PLAYER);			// �R���X�g���N�^
	~CPlayer();															// �f�X�g���N�^
	HRESULT Init(void);													// ����������
	void Uninit(void);													// �I������
	void Update(void);													// �X�V����
	void Draw(void);													// �`�揈��
	void PlayerMotionState(bool *bAction);								// ���[�V�����X�e�[�g�̍X�V
	void PlayerMotion(void);											// ���[�V�����̐ݒ�
	D3DXVECTOR3 GetPos(void) { return m_pos; }			// �ʒu�̎擾
	D3DXVECTOR3 GetRot(void) { return m_rot; }			// �����̎擾
	D3DXVECTOR3 GetMove(void) { return m_move; }		// �ړ��ʂ̎擾
	PLAYERSTATE GetState(void) { return m_state; }		// �X�e�[�g�̎擾
	char * ReadLine(FILE *pFile, char *pDst);			// 1�s���ǂݍ��ނ��߂̊֐�
	char *GetLineTop(char *pSrc);						// ������̐擪��ݒ�
	int PopString(char *pStr, char *pDst);				// ������𔲂��o��
	static bool GetNengouUp(void) { return m_bNengouUp; }		//	�N�����グ���擾
	static bool GetDisp(void) { return m_bDisp; }				//	���S�擾

	// �ÓI�����o�֐�
	static CPlayer * Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);			// �v���C���[�̐���

private:
	// ��{�g�p����ϐ�-------------------------------------------------------------------------------------------------
	LPD3DXMESH			m_pMesh;													// ���b�V�����i���_���j�ւ̃|�C���^
	LPD3DXBUFFER		m_pBuffMat;												// �}�e���A�����ւ̃|�C���^
	DWORD				m_nNumMat;											// �}�e���A�����̐�
	D3DXMATRIX			m_mtxWorld;												// ���[���h�}�g���b�N�X
	D3DXVECTOR3			m_pos;														// ���݂̈ʒu
	D3DXVECTOR3			m_posOld;													// �O��̈ʒu
	D3DXVECTOR3			m_rot;														// ����
	D3DXVECTOR3			m_scale;													// ����
	D3DXVECTOR3			m_move;														// �ړ���
	float				m_fAngle;									// �p�x�����߂�
	float				m_fDestAngle;
	int					m_StateCnt;										//��ԕω��̃J�E���^�[
	PLAYERSTATE			m_state;													//�v���C���[�̏��
	bool				m_bDraw;									//�`�悷�邩
	int					m_nLife;										// �v���C���[�̃��C�t
	float				m_nCntSpeed;									//	�v���C���[�̓���
	// ���b�V���t�B�[���h�̔���Ŏg�p����ϐ�------------------------------------------------------------------------
	D3DXVECTOR3			m_vecA;														//��
	D3DXVECTOR3			m_vecC;														//aPos����pos�܂ł̃x�N�g��
	float				m_fAnswer;									// �x�N�g���̓���
	// ���f���̓ǂݍ��݂Ŏg�p����ϐ�----------------------------------------------------------------------------------
	CModel							*m_apModel[MAX_PLAYER_PARTS];						// ���f���ւ̃|�C���^
	char							m_aFilenameModel[MAX_PLAYER_PARTS][256];	// �ǂݍ��񂾃��f������ۑ����Ă����ϐ�
	int								m_nNumModel;									// �ǂݍ��ރ��f���̐�
	int								m_nNumParts;									// �ǂݍ��ރ��f���̃p�[�c��
	int								m_aIndexParent[MAX_PLAYER_PARTS];				// �p�[�c�̃C���f�b�N�X�ԍ�
	KEY								m_aKeyOffset[MAX_PLAYER_PARTS];					// �L�[�̃I�t�Z�b�g
	static D3DXVECTOR3				m_PartsPos[MAX_PLAYER_PARTS];								// �p�[�c�̈ʒu
	static D3DXVECTOR3				m_PartsRot[MAX_PLAYER_PARTS];								// �p�[�c�̌���
	static int						m_aParent[MAX_PLAYER_PARTS];					//�p�[�c�̐e
	static int						m_aIndex[MAX_PLAYER_PARTS];						//�p�[�c�̔ԍ�
	// ���[�V�����Ŏg�p����ϐ�----------------------------------------------------------------------------------
	static D3DXVECTOR3	m_PartsDefaultPos[MAX_PLAYER_PARTS];						// �f�t�H���g�̃p�[�c�̈ʒu
	static D3DXVECTOR3	m_PartsDefaultRot[MAX_PLAYER_PARTS];						// �f�t�H���g�̃p�[�c�̌���
	static D3DXVECTOR3	m_motionPos[MAX_PLAYER_PARTS];								// ���[�V������������p�ʒu
	static D3DXVECTOR3	m_motionRot[MAX_PLAYER_PARTS];								// ���[�V���������p����p����
	int					m_nNumKey;											// �L�[�̑���
	KEY_INFO			*m_pKeyInfo;										// �L�[���̃|�C���^
	KEY_INFO			m_aKeyInfo[MOTIONSTATE_MAX];						// �L�[���
	MOTIONSTATE			m_motionState;												// ���[�V�����̏��
	MOTIONSTATE			m_motionStateOld;											// �O��̃��[�V�������
	bool				m_bAction[PLAYERBOOLTYPE_MAX];					// �s�����̃t���O
	bool				m_motionBlend;									// �u�����h���邩���Ȃ���
	int					m_nKey;											// ���݂̃L�[
	int					m_nAllFrame;									// �S�t���[����
	int					m_nCntMotion;									// ���[�V�����J�E���^�[
	int					m_nCntFrame;									// �t���[���J�E���^�[
	int					m_nCntAllFrame;									// �S�t���[���̃J�E���^�[
	int					m_nCntKey;										// �L�[�̃J�E���^�[
	int					m_nFrame;										// ���݂̃t���[��
	float				m_fRadius;										// �����蔻��͈̔�
	int					m_nCountFream;									//	�J�E���^�[�t���[��
	static bool			m_bNengouUp;									//	�����_��
	static bool			m_bDisp;										//	���S���

};
#endif