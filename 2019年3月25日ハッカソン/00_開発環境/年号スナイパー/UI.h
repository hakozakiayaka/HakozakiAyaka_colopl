//=============================================================================
//
// UI���� [ui.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene2d.h"	// �V�[��2D
#include "manager.h"	// �}�l�[�W���[

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define UI_TEXMAX		(13)

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CPause;
class CBullet3D;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CUI : public CScene2D
{// UI �N���X
public:
	CUI();											// �R���X�g���N�^
	~CUI();											// �f�X�g���N�^

	static HRESULT Load(CManager::MODE mode);		// ���[�h
	static void Unload(void);						// �A�����[�h
													// ����
	static CUI *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 uv, int nType);
	// ����������
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 uv, int nType);
	void Uninit(void);								// �I������
	void Update(void);								// �X�V����
	void Draw(void);								// �`�揈��
	static void CreateMode(CManager::MODE mode);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture[UI_TEXMAX];		//  �e�N�X�`�����ւ̃|�C���^
	int m_nType;											//  ���
	D3DXVECTOR3 m_sizeOld;									//	�����̑傫���̎擾
	static CManager::MODE m_mode;							//	�Q�[���̃��[�h
	bool m_bFlash;											//	�_��
	int m_nCounter;											//	�J�E���^�[
	bool m_bPressButton;									//	�{�^�����������ǂ���
protected:

};

#endif