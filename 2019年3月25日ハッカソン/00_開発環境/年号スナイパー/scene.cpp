//=============================================================================
//
// シーン処理 [scene.cpp]
// Author :YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"		// シーン
#include "manager.h"	// マネージャー
#include "renderer.h"	// レンダラー
#include "input.h"		// 入力

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CScene		*CScene::m_pTop[PRIORITY_MAX] = {};			// 先頭のオブジェクトへのポインタ
CScene		*CScene::m_pCur[PRIORITY_MAX] = {};			// 現在のオブジェクトへのポインタ

int			CScene::m_nNumAll = 0;						// 総数
int			CScene::m_nNumPri[PRIORITY_MAX] = {};		// 総数
int			CScene::m_nStartUp = 0;						// 更新範囲

//=============================================================================
// コンストラクタ
//=============================================================================
CScene::CScene(int nPriority, OBJTYPE objType)
{
	m_pPrev = m_pCur[nPriority];						// 前のオブジェクト設定
	m_pNext = NULL;										// 次のオブジェクト設定

	m_pCur[nPriority] = this;							// 現在のオブジェクト設定

	if (m_pPrev != NULL)
	{// 前のオブジェクトがNULL以外の場合
		m_pPrev->m_pNext = m_pCur[nPriority];			// 前の次のオブジェクト設定
	}

	m_nPriority = nPriority;							// 順位設定
	m_objType = objType;								// 種類
	m_bDeath = false;									// 死亡フラグ
	m_nNumAll++;										// 総数カウント
	m_nNumPri[nPriority]++;								// 総数カウント

	if (m_pTop[nPriority] == NULL)
	{// 先頭のオブジェクトがNULL以外の場合
		m_pTop[nPriority] = m_pCur[nPriority];			// 先頭のオブジェクト設定
	}
	else if (m_pTop[nPriority] == m_pCur[nPriority]->m_pPrev)
	{// 先頭のオブジェクトと前のオブジェクトが同じ場合
		m_pTop[nPriority]->m_pNext = m_pCur[nPriority];	// 先頭の次のオブジェクト設定
	}

}

//=============================================================================
// デストラクタ
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// 全フェード以外開放処理
//=============================================================================
void CScene::ReleaseMode(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_FADE; nCntPriority++)
	{// カウント
		while (m_pTop[nCntPriority] != NULL)
		{// 先頭のオブジェクトがNULLになるまで
			m_pTop[nCntPriority]->Uninit();			// 先頭のオブジェクト終了
			m_pTop[nCntPriority]->Release();		// 開放処理
		}
		m_nNumPri[nCntPriority] = 0;				// プライオリティ総数初期化
	}

	m_nNumAll = 0;									// 総数初期化
}

//=============================================================================
// 全開放処理
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{// カウント
		while (m_pTop[nCntPriority] != NULL)
		{// 先頭のオブジェクトがNULLになるまで
			m_pTop[nCntPriority]->Uninit();			// 先頭のオブジェクト終了
			m_pTop[nCntPriority]->Release();		// 開放処理
		}
		m_nNumPri[nCntPriority] = 0;				// プライオリティ総数初期化
	}

	m_nNumAll = 0;									// 総数初期化
}

//=============================================================================
// 全更新処理
//=============================================================================
void CScene::UpadteAll(void)
{
	for (int nCntPriority = m_nStartUp; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		CScene *pScene = m_pTop[nCntPriority];		// 更新のオブジェクト（先頭を記憶）
		CScene *pNext = NULL;						// 次のオブジェクト記憶

		while (pScene != NULL)
		{// 更新のオブジェクトがNULL以外の場合
			pNext = pScene->m_pNext;				// 次のオブジェクト設定
			if (pScene->m_bDeath == false)
			{// 破棄しない
				pScene->Update();					// 更新処理
			}
			if (pScene->m_bDeath == true)
			{// 破棄する
				pScene->Release();					// 開放処理
			}
			pScene = pNext;							// 更新のオブジェクトに次のオブジェクト設定
		}
	}
}

//=============================================================================
// 全描画処理
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{// カウント
		CScene *pScene = m_pTop[nCntPriority];		// 描画のオブジェクト（先頭を記憶）
		CScene *pNext = NULL;						// 次のオブジェクト記憶

		while (pScene != NULL)
		{// 描画のオブジェクトがNULL以外の場合
			pNext = pScene->m_pNext;				// 次のオブジェクト設定
			if (pScene->m_bDeath == false)
			{// 破棄しない
				pScene->Draw();						// 描画処理
			}
			if (pScene->m_bDeath == true)
			{// 破棄する
				pScene->Release();					// 開放処理
			}
			pScene = pNext;							// 描画のオブジェクトに次のオブジェクト設定
		}
	}
}

//=============================================================================
// 開放処理
//=============================================================================
void CScene::Release(void)
{
	if (this != NULL)
	{// 自分がNULL以外の場合
		int nPriority = m_nPriority;
		OBJTYPE objType = m_objType;

		if (m_pPrev != NULL)	// m_pTop以外
		{// 前のオブジェクトがNULL以外の場合
			if (m_pTop[nPriority] == m_pPrev)
			{// 先頭のオブジェクトが前のオブジェクトと同じ場合
				m_pTop[nPriority]->m_pNext = m_pNext;		// 先頭の次のオブジェクト設定
			}

			m_pPrev->m_pNext = m_pNext;						// 前の次のオブジェクト設定
		}

		if (m_pNext != NULL)	// m_pCur以外
		{// 次のオブジェクトがNULL以外の場合
			if (m_pCur[nPriority] == m_pNext)
			{// 現在のオブジェクトが前のオブジェクトと同じ場合
				m_pCur[nPriority]->m_pPrev = m_pPrev;		// 現在の前のオブジェクト設定
			}

			m_pNext->m_pPrev = m_pPrev;						// 次の前のオブジェクト設定
		}

		if (m_pTop[nPriority] == this)
		{// 先頭のオブジェクトが自分と同じ場合
			m_pTop[nPriority] = m_pNext;					// 先頭のオブジェクトを次に更新
		}
		if (m_pCur[nPriority] == this)
		{// 現在のオブジェクトが自分と同じ場合
			m_pCur[nPriority] = m_pPrev;					// 現在のオブジェクトを前に更新
		}

		m_pPrev = NULL;										// 前のオブジェクト
		m_pNext = NULL;										// 次のオブジェクト


		delete this;										// 破棄
		m_nNumPri[nPriority]--;								// プライオリティ総数を減らす
		m_nNumAll--;										// 総数を減らす
	}
}
