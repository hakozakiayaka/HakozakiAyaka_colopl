//=============================================================================
//
// フェード処理 [fade.h]
// Author : YUTARO ABE
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"		// メイン
#include "scene2d.h"	// シーン2D
#include "manager.h"	// マネージャー

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_TEX (2)		//テクスチャの最大数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CFade : public CScene2D
{// フェード（親：CScene2D）
public:
	typedef enum
	{// フェード状態
		FADE_NONE = 0,		// 何もしていない状態
		FADE_IN,			// フェードイン状態
		FADE_OUT,			// フェードアウト状態
		FADE_MAX
	} FADE;

	CFade();												// コンストラクタ
	~CFade();												// デストラクタ

	static HRESULT Load(void);								// ロード
	static void Unload(void);								// アンロード
	static CFade *Create(CManager::MODE mode, FADE fade,int nType);	// 生成

	HRESULT Init(CManager::MODE mode, FADE fade);			// 初期化処理
	void Uninit(void);										// 終了処理
	void Update(void);										// 更新処理
	void Draw(void);										// 描画処理

	static FADE GetFade(void) { return m_fade; }			// フェード取得

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEX];				// テクスチャ情報へのポインタ

	CManager::MODE m_modeNext;								// モード次回
	static FADE m_fade;										// フェード
	int m_nCntTimer;										// 時間カウンタ

protected:

};

#endif