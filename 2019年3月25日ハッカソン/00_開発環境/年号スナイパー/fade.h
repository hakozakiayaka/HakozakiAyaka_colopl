//=============================================================================
//
// �t�F�[�h���� [fade.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"		// ���C��
#include "scene2d.h"	// �V�[��2D
#include "manager.h"	// �}�l�[�W���[

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TEX (2)		//�e�N�X�`���̍ő吔

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CFade : public CScene2D
{// �t�F�[�h�i�e�FCScene2D�j
public:
	typedef enum
	{// �t�F�[�h���
		FADE_NONE = 0,		// �������Ă��Ȃ����
		FADE_IN,			// �t�F�[�h�C�����
		FADE_OUT,			// �t�F�[�h�A�E�g���
		FADE_MAX
	} FADE;

	CFade();												// �R���X�g���N�^
	~CFade();												// �f�X�g���N�^

	static HRESULT Load(void);								// ���[�h
	static void Unload(void);								// �A�����[�h
	static CFade *Create(CManager::MODE mode, FADE fade,int nType);	// ����

	HRESULT Init(CManager::MODE mode, FADE fade);			// ����������
	void Uninit(void);										// �I������
	void Update(void);										// �X�V����
	void Draw(void);										// �`�揈��

	static FADE GetFade(void) { return m_fade; }			// �t�F�[�h�擾

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEX];				// �e�N�X�`�����ւ̃|�C���^

	CManager::MODE m_modeNext;								// ���[�h����
	static FADE m_fade;										// �t�F�[�h
	int m_nCntTimer;										// ���ԃJ�E���^

protected:

};

#endif