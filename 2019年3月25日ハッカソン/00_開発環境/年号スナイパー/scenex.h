//=============================================================================
//
// �V�[��X���� [scenex.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _SCENEX_H_
#define _SCENEX_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene.h"		// �V�[��

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSceneX : public CScene
{// ���f��3D�i�e�FCScene�j
public:
	CSceneX(int nPriority, CScene::OBJTYPE objType);// �R���X�g���N�^
	~CSceneX();										// �f�X�g���N�^

	HRESULT Init(void);								// ����������
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��

	void SetInitAll(LPDIRECT3DTEXTURE9 *pTexture, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
	{// �����l�ݒ�
		m_pTexture = pTexture;			// �e�N�X�`��
		m_pMesh = pMesh;				// ���b�V��
		m_pBuffMat = pBuffMat;			// �}�e���A���o�b�t�@
		m_nNumMat = nNumMat;			// �}�e���A����
		m_pos = pos;					// �ʒu
		m_rot = rot;					// ����
		m_scale = scale;				// �傫���̎擾
	}
	void Setpos(D3DXVECTOR3 pos) { m_pos = pos; }			// �ʒu�ݒ�
	D3DXVECTOR3 Getpos(void) { return m_pos; }				// �ʒu�擾
	void Setrot(D3DXVECTOR3 rot) { m_rot = rot; }			// �����ݒ�
	D3DXVECTOR3 Getrot(void) { return m_rot; }				// �����擾
	void SetScale(D3DXVECTOR3 scale) { m_scale = scale; }	// �傫���ݒ�
	D3DXVECTOR3 GetScale(void) { return m_scale; }			// �傫���擾

	D3DXMATRIX GetmtxWorld(void) { return m_mtxWorld; }	// �}�g���b�N�X�̎擾
	D3DXVECTOR3 Getsize(int nNum){if (nNum == 0){return m_vtxMax;}else{return m_vtxMin;}}		// �傫���擾
	void Setcol(D3DXCOLOR col) { m_col = col; m_bcolChange = true; }							// �F�ݒ�
private:
	LPDIRECT3DTEXTURE9	*m_pTexture = NULL;				// �e�N�X�`�����ւ̃|�C���^
	LPD3DXMESH			m_pMesh = NULL;					// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER		m_pBuffMat = NULL;				// �}�e���A�����ւ̃|�C���^
	DWORD				m_nNumMat = NULL;				// �}�e���A�����̐�
	D3DXMATRIX			m_mtxWorld;						// ���[���h�}�g���b�N�X

	D3DXVECTOR3			m_vtxMin;						// ���_�̍ŏ��l
	D3DXVECTOR3			m_vtxMax;						// ���_�̍ő�l

	D3DXVECTOR3			m_pos;							// �ʒu
	D3DXVECTOR3			m_rot;							// ����
	D3DXCOLOR			m_col;							// �F
	bool				m_bcolChange;					// �F�ύX
	D3DXVECTOR3			m_scale;						 //�g��k���p

protected:

};

#endif