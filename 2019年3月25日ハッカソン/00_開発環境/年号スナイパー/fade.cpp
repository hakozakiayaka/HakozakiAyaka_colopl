//=============================================================================
//
// フェード処理 [fade.cpp]
// Author :YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "fade.h"			// フェード
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME000		"data\\TEXTURE\\FADE\\fade000.png"		// テクスチャ
#define TEXTURE_NAME001		"data\\TEXTURE\\FADE\\fade001.png"		// テクスチャ
#define ADDCOLOR_A			(1.0f / 60.0f)							// フェード時間

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9			CFade::m_pTexture[MAX_TEX] = {};				// テクスチャ情報へのポインタ
CFade::FADE					CFade::m_fade			   = CFade::FADE_NONE;	// フェード状態

//=============================================================================
// コンストラクタ
//=============================================================================
CFade::CFade() : CScene2D(CScene::PRIORITY_FADE, CScene::OBJTYPE_FADE)
{
	// 初期値設定
	m_nCntTimer = 0;		// タイマー
}

//=============================================================================
// デストラクタ
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
// ロード処理
//=============================================================================
HRESULT CFade::Load(void)
{
		// デバイス取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
		// テクスチャの生成
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME000, &m_pTexture[0]);
		D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME001, &m_pTexture[1]);

	return S_OK;
}

//=============================================================================
// アンロード処理
//=============================================================================
void CFade::Unload(void)
{
	for (int nCount = 0; nCount < MAX_TEX; nCount++)
	{//	テクスチャの最大数
		// テクスチャの破棄
		if (m_pTexture[nCount] != NULL)
		{// NULL以外の場合
			m_pTexture[nCount]->Release();		// 解放
			m_pTexture[nCount] = NULL;			// NULLへ
		}
	}
}

//=============================================================================
// 確保処理
//=============================================================================
CFade *CFade::Create(CManager::MODE mode, FADE fade, int nType)
{
	CFade *pFade = NULL;					// ポインタ

	if (m_fade == FADE_NONE)
	{// フェードしていない
		if (pFade == NULL)
		{// NULLの場合
			pFade = new CFade;				// メモリ確保
			if (pFade != NULL)
			{// NULL以外の場合
				pFade->Init(mode, fade);				// 初期化処理
				if (nType < MAX_TEX)
				{// テクスチャ設定
					pFade->BindTexture(m_pTexture[nType]);
				}
				else
				{// テクスチャ設定
					pFade->BindTexture(NULL);
				}
			}
		}
	}
	return pFade;	// 値を返す
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CFade::Init(CManager::MODE mode, FADE fade)
{
	if (fade == FADE_OUT)
	{// フェードアウト
		CScene2D::SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));		// 初期値設定
	}
	else
	{// その他
		CScene2D::SetInitAll(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));		// 初期値設定
	}

	CScene2D::Init();		// 初期化処理

	m_fade = FADE_OUT;		// フェードアウト
	m_modeNext = mode;		// 次モード
	m_nCntTimer = 0;		// カウント

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CFade::Uninit(void)
{
	CScene2D::Uninit();						// 終了処理
}

//=============================================================================
// 更新処理
//=============================================================================
void CFade::Update(void)
{
	D3DXCOLOR col = CScene2D::Getcol();		// 色を取得
	bool bDelete = false;					// 破棄する

	if (m_fade != FADE_NONE)
	{// フェード使われている
		if (m_fade == FADE_IN)
		{// フェードイン
			col.a -= ADDCOLOR_A;			// 透明に

			if (col.a <= 0.0f)
			{// 透明
				col.a = 0.0f;				// 透明度を調整
				m_fade = FADE_NONE;			// 何もしていない状態
				m_nCntTimer = 0;			// カウント初期化

				bDelete = true;				// 破棄する
			}
		}
		else if (m_fade == FADE_OUT)
		{// フェードアウト
			col.a += ADDCOLOR_A;			// 不透明に

			if (col.a >= 1.0f)
			{// 不透明
				col.a = 1.0f;				// 透明度を調整

				m_nCntTimer++;				// カウンタを加算
				if (m_nCntTimer > 30)
				{// 一定フレーム語
					m_fade = FADE_IN;		// フェードイン
				}
				else if (m_nCntTimer == 15)
				{// フレームの中間
					CManager::SetMode(m_modeNext);		// モード変更
					CScene::SetStartUp(0);				// 更新範囲を初期化
				}
			}
		}
	}

	CScene2D::Setcol(col);					// 色を設定
	CScene2D::Update();						// 更新処理

	if (bDelete == true)
	{// 破棄する
		Uninit();							// 終了処理
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CFade::Draw(void)
{
	CScene2D::Draw();						// 描画処理
}