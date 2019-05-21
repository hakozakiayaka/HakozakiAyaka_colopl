//=============================================================================
//
// サウンド処理 [sound.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=============================================================================
//	サウンドクラス
//=============================================================================
class CSound
{
public:
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;

	typedef enum
	{// サウンドファイル
		SOUND_LABEL_BGM_TITLE = 0,		// タイトル
		SOUND_LABEL_BGM_TUTORIAL,		// チュートリアル
		SOUND_LABEL_BGM_GAME,			// ゲーム
		SOUND_LABEL_BGM_RANKING,		// ランキング

		SOUND_LABEL_SE_BULLET,			// 弾発射音
		SOUND_LABEL_SE_PRESS,			// 決定音
		SOUND_LABEL_SE_SHUTTER,			// シャッター音
		SOUND_LABEL_MAX,
	} SOUND_LABEL;
	CSound();						//	コンストラクタ
	~CSound();						//	デストラクタ
	static HRESULT InitSound(HWND hWnd);
	static void UninitSound(void);
	static HRESULT PlaySound(SOUND_LABEL label);
	static void StopSound(SOUND_LABEL label);
	void StopSound(void);
	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:

	static IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのポインタ
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイスへのポインタ
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイスへのポインタ
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX] ;					// オーディオデータへのポインタ
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX] ;					// オーディオデータサイズ
																	// 各音素材のパラメータ


};

#endif
