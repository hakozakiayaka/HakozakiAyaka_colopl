//=============================================================================
//
// マネージャー処理 [manager.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "camera.h"			// カメラ
#include "light.h"			// ライト
#include "renderer.h"		// レンダラー
#include "fade.h"			// フェード
#include "title.h"			// タイトル
#include "game.h"			// ゲーム
#include "result.h"			// リザルト
#include "ranking.h"		// ランキング
#include "scene.h"			// シーン
#include "debugproc.h"		// デバッグログ
#include "sound.h"			// サウンド
#include "tutorial.h"		// チュートリアル

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define STATE_MODE (MODE_TITLE)		//	始まるモード

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
CManager::MODE		CManager::m_mode			= CManager::MODE_TITLE;	// モード
CInputKeyboard		*CManager::m_pInputKeyboard = NULL;					// キーボード
CInputDIPad			*CManager::m_pInputDIPad	= NULL;					// DIパッド
CInputXPad			*CManager::m_pInputXPad		= NULL;					// Xパッド
CRenderer			*CManager::m_pRenderer		= NULL;					// レンダラー
CCamera				*CManager::m_pCamara		= NULL;					// カメラ
CLight				*CManager::m_pLight			= NULL;					// ライト
CTitle				*CManager::m_pTitle			= NULL;					// タイトル
CTutorial			*CManager::m_pTutorial		= NULL;					// チュートリアル
CSelectMode			*CManager::m_pSelectMode	= NULL;					// セレクトモード
CGame				*CManager::m_pGame			= NULL;					// ゲーム
CResult				*CManager::m_pResult		= NULL;					// リザルト
CDebugProcLEFT *CManager::m_CDebugProcLeft		= NULL;					// デバッグ用の左用文字列
CDebugProcRIGHT *CManager::m_CDebugProcRight	= NULL;					// デバッグ用の右用文字列
CSound				*CManager::m_pSound			= NULL;					// サウンド
int					CManager::m_nPlayerLife = 0;						//	プレイヤーのHP

//ランキング関する処理
CRanking			*CManager::m_pRanking = NULL;					// 順位

int CManager::nRank[5] = { 200,300,400,500,600 };
int CManager::m_LastScore = 0;//初期スコア(基本は0スタートで)
int CManager::m_LastTime = 999999 * 1;
//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager()
{
	m_nPlayerLife = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// キーボード
		if (m_pInputKeyboard == NULL)
		{// NULLの場合//// キーボードメモリ確保
			m_pInputKeyboard = new CInputKeyboard;
			if (m_pInputKeyboard != NULL)
			{// NULL以外の場合
			 // キーボード初期化処理
				if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
				{
					return -1;
				}
			}
			else
			{// NULLの場合
				MessageBox(0, "InputKeyboardがNULL", "警告", MB_OK);
			}
		}
		else
		{// NULL以外の場合
			MessageBox(0, "InputKeyboardがNULLでない", "警告", MB_OK);
		}
	// DIパッド
		if (m_pInputDIPad == NULL)
		{// NULLの場合// DIパッドメモリ確保
			m_pInputDIPad = new CInputDIPad;
			if (m_pInputDIPad != NULL)
			{// NULL以外の場合
			 // DIパッド初期化処理
				if (FAILED(m_pInputDIPad->Init(hInstance, hWnd)))
				{
					return -1;
				}
			}
			else
			{// NULLの場合
				MessageBox(0, " InputDIPadがNULL", "警告", MB_OK);
			}
		}
		else
		{// NULL以外の場合
			MessageBox(0, "InputDIPadがNULLでない", "警告", MB_OK);
		}
	// DIパッド
		if (m_pInputXPad == NULL)
		{// NULLの場合// DIパッドメモリ確保
			m_pInputXPad = new CInputXPad;
			if (m_pInputXPad != NULL)
			{// NULL以外の場合
			 // DIパッド初期化処理
				if (FAILED(m_pInputXPad->Init(hInstance, hWnd)))
				{
					return -1;
				}
			}
			else
			{// NULLの場合
				MessageBox(0, "InputXPadがNULL", "警告", MB_OK);
			}
		}
		else
		{// NULL以外の場合
			MessageBox(0, " InputXPadがNULLでない", "警告", MB_OK);
		}
		// レンダラー
		if (m_pRenderer == NULL)
		{// NULLの場合// レンダラーメモリ確保
			m_pRenderer = new CRenderer;
			if (m_pRenderer != NULL)
			{// NULL以外の場合
			 // レンダラー初期化処理
				if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
				{
					return -1;
				}
			}
			else
			{// NULLの場合,
				MessageBox(0, "RendererがNULL", "警告", MB_OK);
			}
		}
		else
		{// NULL以外の場合
			MessageBox(0, "RendererがNULLでない", "警告", MB_OK);
		}
		if (m_pSound == NULL)
		{//	サウンドが使用されていなかった場合
			m_pSound = new CSound;
			if (m_pSound != NULL)
			{//	サウンドの生成
				m_pSound->InitSound(hWnd);
			}
			else
			{// NULLの場合
				MessageBox(0, " サウンドがNULL", "警告", MB_OK);
			}
		}
		else
		{// NULL以外の場合
			MessageBox(0, " サウンドがNULLでない", "警告", MB_OK);
		}

		// ライト
		if (m_pLight == NULL)
		{// NULLの場合// ライトメモリ確保
			m_pLight = new CLight;
			if (m_pLight != NULL)
			{// NULL以外の場合
			 // ライト初期化処理
				if (FAILED(m_pLight->Init()))
				{
					return -1;
				}
			}
			else
			{// NULLの場合
				MessageBox(0, " LightがNULL", "警告", MB_OK);
			}
		}
		else
		{// NULL以外の場合
			MessageBox(0, " LightがNULLでない", "警告", MB_OK);
		}
		if (m_CDebugProcLeft == NULL)
		{//	左文字列の生成
			m_CDebugProcLeft = new CDebugProcLEFT;
			if (m_CDebugProcLeft != NULL)
			{//	左文字列が生成されている
				m_CDebugProcLeft->Init();
			}
		}
		if (m_CDebugProcRight == NULL)
		{//	右文字列の生成
			m_CDebugProcRight = new CDebugProcRIGHT;
			if (m_CDebugProcRight != NULL)
			{//	右文字列が生成されている
				m_CDebugProcRight->Init();
			}
		}
	//	フェイドの読み込み
	CFade::Load();
	SetMode(STATE_MODE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{

	switch (m_mode)
	{// モード
	 // タイトル----------------------------
	case CManager::MODE_TITLE:
		if (m_pTitle != NULL)
		{// 終了処理
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;
		// チュートリアル----------------------------
	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// 終了処理
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;
		// ゲームステージ1----------------------------
	case CManager::MODE_GAME:
		if (m_pGame != NULL)
		{// 終了処理
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		break;
		// ランキング----------------------------
	case CManager::MODE_RANKING:
		if (m_pRanking != NULL)
		{// 終了処理
			m_pRanking->Uninit();
			delete m_pRanking;
			m_pRanking = NULL;
			m_LastScore = 0;//ランキングが終わり次第再度数値をリセット
			m_LastTime = 999999 * 1;//同じくタイムの初期化

		}
		break;

	}
	//	フェイドの開放
	CFade::Unload();

	if (m_pInputKeyboard != NULL)
	{// NULL以外の場合
		// 終了処理
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;	// キーボードメモリ開放
		m_pInputKeyboard = NULL;	// NULL
	}

	if (m_pInputDIPad != NULL)
	{// NULL以外の場合
		// 終了処理
		m_pInputDIPad->Uninit();
		delete m_pInputDIPad;	// DIパッドメモリ開放
		m_pInputDIPad = NULL;	// NULL
	}
	if (m_pInputXPad != NULL)
	{// NULL以外の場合
	 // 終了処理
		m_pInputXPad->Uninit();
		delete m_pInputXPad;	// Xパッドメモリ開放
		m_pInputXPad = NULL;	// NULL
	}

	// レンダラー
	if (m_pRenderer != NULL)
	{// NULL以外の場合
		// 終了処理
		m_pRenderer->Uninit();
		delete m_pRenderer;	// レンダラーメモリ開放
		m_pRenderer = NULL;	// NULL
	}

	// カメラ
	if (m_pCamara != NULL)
	{// NULL以外の場合
		// 終了処理
		m_pCamara->Uninit();
		delete m_pCamara;	// カメラメモリ開放
		m_pCamara = NULL;	// NULL
	}
	if (m_pSound != NULL)
	{// NULL以外の場合
		//終了処理
		m_pSound->UninitSound();
		delete m_pSound;	//	サウンドの開放
		m_pSound = NULL;	//	NULL
	}
	// ライト
	if (m_pLight != NULL)
	{// NULL以外の場合
		// 終了処理
		m_pLight->Uninit();

		delete m_pLight;	// ライトメモリ開放
		m_pLight = NULL;	// NULL
	}

	if (m_CDebugProcLeft != NULL)
	{
		//	デバッグ用文字フォントの終了処理
		m_CDebugProcLeft->Uninit();
		//メモリの開放
		delete m_CDebugProcLeft;
		//空にする
		m_CDebugProcLeft = NULL;
	}
	if (m_CDebugProcRight != NULL)
	{
		//	デバッグ用文字フォントの終了処理
		m_CDebugProcRight->Uninit();
		//メモリの開放
		delete m_CDebugProcRight;
		//空にする
		m_CDebugProcRight = NULL;
	}

}

//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	switch (m_mode)
	{// モード
	 // タイトル----------------------------
	case CManager::MODE_TITLE:
		if (m_pTitle != NULL)
		{// 更新処理
			m_pTitle->Update();
		}
		break;
		// チュートリアル----------------------------
	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// 終了処理
			m_pTutorial->Update();
		}
		break;
		// ステージ1------------------------------
	case CManager::MODE_GAME:
		if (m_pGame != NULL)
		{// 更新処理
			m_pGame->Update();
		}
		break;
		// ランキング------------------------------
	case CManager::MODE_RANKING:
		if (m_pRanking != NULL)
		{// 更新処理
			m_pRanking->Update();
		}
		break;

	}
	if (m_pInputKeyboard != NULL)
	{// NULL以外の場合
		// キーボード更新処理
		m_pInputKeyboard->Update();
	}

	if (m_pInputDIPad != NULL)
	{// NULL以外の場合
		// DIパッド更新処理
		m_pInputDIPad->Update();
	}

	if (m_pInputXPad != NULL)
	{// NULL以外の場合
		// Xパッド更新処理
		m_pInputXPad->Update();
	}

	if (m_pRenderer != NULL)
	{// NULL以外の場合
		// レンダラー更新処理
		m_pRenderer->Update();
	}

	if (m_pCamara != NULL)
	{// NULL以外の場合
		// カメラ更新処理
		m_pCamara->Update();
	}

	if (m_pLight != NULL)
	{// NULL以外の場合
		// ライト更新処理
		m_pLight->Update();
	}


}

//=============================================================================
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	if (m_pCamara != NULL)
	{// NULL以外の場合
	 // カメラ描画処理
		m_pCamara->SetCamera();
	}
	if (m_pRenderer != NULL)
	{// NULL以外の場合
	 // レンダラー描画処理
		m_pRenderer->Draw();
	}
}

//=============================================================================
// モード設定処理
//=============================================================================
void CManager::SetMode(CManager::MODE mode)
{

	switch (m_mode)
	{// モード（現在）
	 // タイトル----------------------------
	case CManager::MODE_TITLE:
		if (m_pTitle != NULL)
		{// 終了処理
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_TITLE);	//	BGM止める
		// カメラ
		if (m_pCamara != NULL)
		{// NULL以外の場合
		 // 終了処理
			m_pCamara->Uninit();
			delete m_pCamara;	// カメラメモリ開放
			m_pCamara = NULL;	// NULL
		}
		break;
		// チュートリアル----------------------------
	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{// 終了処理
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_TUTORIAL);
		break;
		// ステージ1---------------------------
	case CManager::MODE_GAME:
		if (m_pGame != NULL)
		{// 終了処理
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		// カメラ
		if (m_pCamara != NULL)
		{// NULL以外の場合
		 // 終了処理
			m_pCamara->Uninit();
			delete m_pCamara;	// カメラメモリ開放
			m_pCamara = NULL;	// NULL
		}
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_GAME);
		break;

		// ランキング---------------------------
	case CManager::MODE_RANKING:
		if (m_pRanking != NULL)
		{// 終了処理
			m_pRanking->Uninit();
			delete m_pRanking;
			m_pRanking = NULL;
			m_LastScore = 0;//ランキングが終わり次第再度数値をリセット
			m_LastTime = 999999 * 1;//同じくタイムの初期化
		}
		m_pSound->StopSound(CSound::SOUND_LABEL_BGM_RANKING);
		// カメラ
		if (m_pCamara != NULL)
		{// NULL以外の場合
		 // 終了処理
			m_pCamara->Uninit();
			delete m_pCamara;	// カメラメモリ開放
			m_pCamara = NULL;	// NULL
		}
		break;

	}


	// モード変更
	m_mode = mode;

	switch (mode)
	{// モード
	 // タイトル----------------------------
	case CManager::MODE_TITLE:
		if (m_pTitle == NULL)
		{// NULLの場合
			m_pTitle = new CTitle;
			if (m_pTitle != NULL)
			{// 初期化処理
				m_LastTime = 999999 * 1;//同じくタイムの初期化

				m_pTitle->Init();
			}
		}
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_TITLE);	//	BGM鳴らす
		if (m_pCamara == NULL)
		{// NULLの場合// カメラメモリ確保
			m_pCamara = new CCamera;
			if (m_pCamara != NULL)
			{// NULL以外の場合
			 // カメラ初期化処理
				m_pCamara->Init(CCamera::CAMERATYPE_TITLE);
			}
		}
		break;
		// チュートリアル----------------------------
	case MODE_TUTORIAL:
		if (m_pTutorial == NULL)
		{// 終了処理
			m_pTutorial = new CTutorial;
			if (m_pTutorial != NULL)
			{// 初期化処理
				m_pTutorial->Init();
			}
		}
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_TUTORIAL);	//	BGM鳴らす
		break;
		// ステージ1----------------------------
	case CManager::MODE_GAME:
		// カメラ
		if (m_pGame == NULL)
		{// NULLの場合
			m_pGame = new CGame;
			if (m_pGame != NULL)
			{// 初期化処理
				m_pGame->Init(CGame::STAGE_001, m_nPlayerLife);
			}
		}
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_GAME);	//	BGM鳴らす
		if (m_pCamara == NULL)
		{// NULLの場合// カメラメモリ確保
			m_pCamara = new CCamera;
			if (m_pCamara != NULL)
			{// NULL以外の場合
			 // カメラ初期化処理
				m_pCamara->Init(CCamera::CAMERATYPE_GAME);
			}
		}
		break;

		// ランキング----------------------------
	case CManager::MODE_RANKING:
		// カメラ
		if (m_pRanking == NULL)
		{// NULLの場合
			m_pRanking = new CRanking;
			if (m_pRanking != NULL)
			{// 初期化処理
				m_pRanking->Init();
			}
		}
		m_pSound->PlaySound(CSound::SOUND_LABEL_BGM_RANKING);
		if (m_pCamara == NULL)
		{// NULLの場合// カメラメモリ確保
			m_pCamara = new CCamera;
			if (m_pCamara != NULL)
			{// NULL以外の場合
			 // カメラ初期化処理
				m_pCamara->Init(CCamera::CAMERATYPE_RANKING);
			}
		}
		break;
	}
}
