//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=============================================================================
//	�T�E���h�N���X
//=============================================================================
class CSound
{
public:
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g(-1�Ń��[�v�Đ�)
	} SOUNDPARAM;

	typedef enum
	{// �T�E���h�t�@�C��
		SOUND_LABEL_BGM_TITLE = 0,		// �^�C�g��
		SOUND_LABEL_BGM_TUTORIAL,		// �`���[�g���A��
		SOUND_LABEL_BGM_GAME,			// �Q�[��
		SOUND_LABEL_BGM_RANKING,		// �����L���O

		SOUND_LABEL_SE_BULLET,			// �e���ˉ�
		SOUND_LABEL_SE_PRESS,			// ���艹
		SOUND_LABEL_SE_SHUTTER,			// �V���b�^�[��
		SOUND_LABEL_MAX,
	} SOUND_LABEL;
	CSound();						//	�R���X�g���N�^
	~CSound();						//	�f�X�g���N�^
	static HRESULT InitSound(HWND hWnd);
	static void UninitSound(void);
	static HRESULT PlaySound(SOUND_LABEL label);
	static void StopSound(SOUND_LABEL label);
	void StopSound(void);
	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:

	static IXAudio2 *m_pXAudio2;									// XAudio2�I�u�W�F�N�g�ւ̃|�C���^
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// �}�X�^�[�{�C�X�ւ̃|�C���^
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X�ւ̃|�C���^
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX] ;					// �I�[�f�B�I�f�[�^�ւ̃|�C���^
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX] ;					// �I�[�f�B�I�f�[�^�T�C�Y
																	// �e���f�ނ̃p�����[�^


};

#endif
