//=============================================================================
//
//	�f�o�b�O�p�����t�H���g�̏��� [DebugProp.h]
// Author :	YUTORO ABE
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//=============================================================================
//	�f�o�b�O�p�N���X
//=============================================================================
class CDebugProc
{
public:
	CDebugProc();										//	�R���X�g���N
	~CDebugProc();										//	�f�X�g���N
	void Init(void);									//	����������
	void Uninit(void);									//	�I������
	void Draw(void);									//	�`��̏���
protected:
	static LPD3DXFONT		m_pFont;					//	�t�H���g�̐���
};

//=============================================================================
//	�f�o�b�O�p���N���X
//=============================================================================
class CDebugProcLEFT : public CDebugProc
{
public:
	CDebugProcLEFT();									//	�R���X�g���N
	~CDebugProcLEFT();									//	�f�X�g���N
	void Init(void);									//	����������
	void Uninit(void);									//	�I������
	void Draw(void);									//	�`��̏���
	static void Print(char *ftm, ...);					//	������̐���
private:
	static  char			m_aStr[1024];				//	CHAR�̕�����
};

//=============================================================================
//	�f�o�b�O�p�E�N���X
//=============================================================================
class CDebugProcRIGHT : public CDebugProc
{
public:
	CDebugProcRIGHT();									//	�R���X�g���N
	~CDebugProcRIGHT();									//	�f�X�g���N
	void Init(void);									//	����������
	void Uninit(void);									//	�I������
	void Draw(void);									//	�`��̏���
	static void Print(char *ftm, ...);					//	������̐���
private:
	static  char			m_aStr[1024];				//	CHAR�̕�����
};

#endif
