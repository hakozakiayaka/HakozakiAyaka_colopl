//=============================================================================
//
//	デバッグ用文字フォントの処理 [DebugProp.h]
// Author :	YUTORO ABE
//
//=============================================================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

#include "main.h"

//=============================================================================
//	デバッグ用クラス
//=============================================================================
class CDebugProc
{
public:
	CDebugProc();										//	コンストラク
	~CDebugProc();										//	デストラク
	void Init(void);									//	初期化処理
	void Uninit(void);									//	終了処理
	void Draw(void);									//	描画の処理
protected:
	static LPD3DXFONT		m_pFont;					//	フォントの生成
};

//=============================================================================
//	デバッグ用左クラス
//=============================================================================
class CDebugProcLEFT : public CDebugProc
{
public:
	CDebugProcLEFT();									//	コンストラク
	~CDebugProcLEFT();									//	デストラク
	void Init(void);									//	初期化処理
	void Uninit(void);									//	終了処理
	void Draw(void);									//	描画の処理
	static void Print(char *ftm, ...);					//	文字列の生成
private:
	static  char			m_aStr[1024];				//	CHARの文字列
};

//=============================================================================
//	デバッグ用右クラス
//=============================================================================
class CDebugProcRIGHT : public CDebugProc
{
public:
	CDebugProcRIGHT();									//	コンストラク
	~CDebugProcRIGHT();									//	デストラク
	void Init(void);									//	初期化処理
	void Uninit(void);									//	終了処理
	void Draw(void);									//	描画の処理
	static void Print(char *ftm, ...);					//	文字列の生成
private:
	static  char			m_aStr[1024];				//	CHARの文字列
};

#endif
