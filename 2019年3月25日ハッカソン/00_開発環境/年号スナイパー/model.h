//=============================================================================
//
// ���f������ [model.h]
// Author : �����ʉ�
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"			// ���C��
#include "scene.h"			// scene

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_PRIORITY				(3)
#define MAX_MODEL					(20)		// �ǂݍ��ސ��������I
#define MAX_CHAR					(256)
#define MODEL_FILENAME		"data/TEXT/model_data.txt"

//*****************************************************************************
// ���f���N���X
//*****************************************************************************
class CModel
{
public:
	typedef enum
	{
		MODELTYPE_PLAYER = 0,		//�v���C���[
		MODELTYPE_ENEMY,			//�G
		MODELTYPE_MAX				//���f���̎�ނ̍ő吔
	}MODELTYPE;

	CModel();																					// �R���X�g���N�^
	~CModel();																					// �f�X�g���N�^
	HRESULT Init(void);																			// ����������
	void Uninit(void);																			// �I������
	void Update(void);																			// �X�V����
	void Draw(void);																			// �`�揈��
	void BindTexture(LPDIRECT3DTEXTURE9 tex);						// �e�N�X�`���̊��蓖��
	void SetPostion(D3DXVECTOR3 pos) { m_rot = pos; }				// �ʒu�̐ݒ�
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }				// �����̐ݒ�
	void SetMatrix(D3DXMATRIX mtx) { m_mtxWorld = mtx; }			// �}�g���b�N�X�̐ݒ�
	void SetParent(CModel * pModel) { m_pParent = pModel; }			// ���f���̐ݒ�
	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }				// ���[���h�}�g���b�N�X�̎擾
	D3DXVECTOR3 GetPosition(void) { return m_pos; }					// �ʒu�̎擾
	D3DXVECTOR3 GetRotation(void) { return m_rot; }					// �����̎擾
	HRESULT Load(LPCSTR Memory);									// �g�p����p�[�c�̓ǂݍ���
	void Unload(void);												// �g�p����p�[�c�̔j��

	// �ÓI�����o�֐�
	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumParts, MODELTYPE type);	//����

private:
	D3DXVECTOR3		m_pos;											// �ʒu
	D3DXVECTOR3		m_rot;											// ����
	D3DXMATRIX		m_mtxWorld;										// ���[���h�}�g���b�N�X
	CModel			*m_pParent;										// �e���f���ւ̃|�C���^
	int				m_nNumParts;
	int				m_nMaxParts;
	MODELTYPE		m_type;											//���f���̎��

	// �ÓI�����o�ϐ�
	static char						m_acFileName[MAX_MODEL][MAX_CHAR];		// �ǂݍ���
	LPD3DXMESH						m_apMesh;			//���b�V�����ւ̃|�C���^
	LPD3DXBUFFER					m_apBuffMat;		//�}�e���A�����ւ̃|�C���^
	DWORD							m_aNumMat;			//�}�e���A���̏��̐�
	static LPDIRECT3DTEXTURE9		m_pTexture;			// �e�N�X�`���ւ̃|�C���^
};
#endif