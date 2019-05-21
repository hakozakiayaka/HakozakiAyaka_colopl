//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"			// ���C��
#include "scene3d.h"		// �V�[��

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_EFFECTTYPE (15)
#define MAX_EFFECT (1000)

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CEffect : public CScene
{// �V�[��3D�i�e�FCScene�j
public:

	typedef struct
	{//	�G�t�F�N�g�̃f�[�^�̎��
		D3DXVECTOR3					posRange[MAX_EFFECT];					// �ʒu�͈̔�
		D3DXVECTOR3					rot[MAX_EFFECT];						// ����
		D3DXVECTOR3					move[MAX_EFFECT];						// ����
		D3DXVECTOR3					size[MAX_EFFECT];						// �傫��
		D3DXCOLOR					col[MAX_EFFECT];						// �F
		D3DXVECTOR2					TexUV[MAX_EFFECT];						// UV
		int							nSetPoriMAX[MAX_EFFECT];				// �|���S���̐�
		float						fGravity[MAX_EFFECT];					// �d��
		int							nLife[MAX_EFFECT];						// �̗�
		int							nBillType[MAX_EFFECT];					// ���Z�������邩�Ȃ������
		int							nCounter;								// �g�p����Ă��鐔�̃J�E���g
	}EFFECT_STATE;

	CEffect();																// �R���X�g���N�^
	~CEffect();																// �f�X�g���N�^
	HRESULT Init(void);														// ����������
	void Uninit(void);														// �I������
	void Update(void);														// �X�V����
	void Draw(void);														// �`�揈��
	static CEffect *Create(D3DXVECTOR3 pos, int nTextureType, int nTxstType);	// ����
	static HRESULT Load(void);												// �e�N�X�`���̓ǂݍ���
	static void Unload(void);												// �j��
	static void LoadDataEffect(void);										// �G�t�F�N�g�̏��ǂݍ���(�f�[�^�ǂݍ���)
	static void LoadNameEffect(void);										// �G�t�F�N�g�̏��ǂݍ���(�e�L�X�g�̖��O�ǂݍ���)
	static void LoadTexNameEffect(void);									// �G�t�F�N�g�̏��ǂݍ���(�G�t�F�N�g�̉摜�̖��O�ǂݍ���)

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[MAX_EFFECTTYPE];					// �e�N�X�`�����ւ̃|�C���^
	D3DXMATRIX					m_mtxWorld;									// ���[���h�}�g���b�N�X
	CScene3D					*m_apScene3D[MAX_EFFECT];					// �V�[��3D�̃|�C���^�^�ϐ�
	static EFFECT_STATE			m_EffectState;								// �G�t�F�N�g�̃X�e�[�^�X

	int							m_nEffectType;								// �G�t�F�N�g�̎��
	D3DXVECTOR3					m_pos;										// �ʒu
	D3DXVECTOR2					m_TexUV;									// UV
	D3DXCOLOR					m_col;										// �F
	D3DXVECTOR3					m_rot;										// ����
	D3DXVECTOR3					m_size;										// �傫��
	int							m_nSetPoriMAX;								// �|���S���̐�
	int							m_nLife;									// �̗�
	D3DXVECTOR3					m_posRange;									// �ʒu�͈̔�
	D3DXVECTOR3					m_move;										// ����
	float						m_fGravity;									// �d��
	D3DXVECTOR3					m_NumMove[MAX_EFFECT];						// �����̊��蓖��
	int							m_nNumLife[MAX_EFFECT];						// �̗͊��蓖�Ă̐ݒ�
	static char					m_cTextName[128];							// �e�L�X�g�̖��O
	static char					m_cTextureName[MAX_EFFECTTYPE][128];		// �e�L�X�g�̖��O
	int							m_nBindText;								// �ǂ̃e�L�X�g���G�t�F�N�g�Ɋ��蓖�Ă邩
	static int					m_nTexttureType;							// �e�N�X�`���̎��
protected:

};

#endif