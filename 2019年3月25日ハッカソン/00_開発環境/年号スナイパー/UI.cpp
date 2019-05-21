//=============================================================================
//
// UI処理 [UIe.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "ui.h"				// タイトルフレーム
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "game.h"			// ゲーム
#include "player.h"			//	プレイヤー
#include "fade.h"			//	フェイド
#include "sound.h"			//	サウンド

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//	#	タイトル
#define	TEXTURE_TITLE000		"data\\TEXTURE\\UI\\TITLE\\title000.png"		// タイトルロゴ000
#define	TEXTURE_BOTTON_ENTER	"data\\TEXTURE\\UI\\BUTTON\\PRESSENTER.png"		// ボタン
//	#	チュートリアル
#define	TEXTURE_TUTORIAL000		"data\\TEXTURE\\UI\\TUTORIAL\\tutorial.png"		// チュートリアル
//	#	ゲーム
#define	TEXTURE_GAME000		"data\\TEXTURE\\UI\\GAME\\game000.png"				// LIVE
#define	TEXTURE_GAME001		"data\\TEXTURE\\UI\\GAME\\game001.png"				// 新年号
#define	TEXTURE_GAME002 	"data\\TEXTURE\\UI\\GAME\\game002.png"				// 速報
#define	TEXTURE_GAME003 	"data\\TEXTURE\\UI\\GAME\\Blood.png"				// 返り血
#define	TEXTURE_GAME004 	"data\\TEXTURE\\UI\\GAME\\nengou000.jpg"			// 年号000
//	#	ランキング
#define	TEXTURE_RANKING001 	"data\\TEXTURE\\UI\\RANKING\\raking001.png"			// 順位001
#define	TEXTURE_RANKING002 	"data\\TEXTURE\\UI\\RANKING\\raking002.png"			// 順位002
#define	TEXTURE_RANKING003 	"data\\TEXTURE\\UI\\RANKING\\raking003.png"			// 順位003
#define	TEXTURE_RANKING004 	"data\\TEXTURE\\UI\\RANKING\\raking004.png"			// 順位004
#define	TEXTURE_RANKING005 	"data\\TEXTURE\\UI\\RANKING\\raking005.png"			// 順位005



//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	CUI::m_pTexture[UI_TEXMAX] = {};			//  テクスチャ情報へのポインタ
CManager::MODE		CUI::m_mode = CManager::MODE_NONE;			//	ゲームのモード

																//=============================================================================
																// コンストラクタ
																//=============================================================================
CUI::CUI() : CScene2D(CScene::PRIORITY_UI, CScene::OBJTYPE_UI)
{
	m_nType = 0;								//	種類
	m_sizeOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//	初期の大きさの位置
	m_bFlash = false;							//	点滅しない
	m_bPressButton = false;					//	ボタンの設定
}

//=============================================================================
// デストラクタ
//=============================================================================
CUI::~CUI()
{

}
//=============================================================================
// ロード処理
//=============================================================================
HRESULT CUI::Load(CManager::MODE mode)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//	ゲームの設定
	m_mode = mode;
	switch (mode)
	{
	case CManager::MODE_TITLE://	タイトル	------------------------------------------------------------------------
		D3DXCreateTextureFromFile(pDevice, TEXTURE_BOTTON_ENTER, &m_pTexture[0]);	//	ボタン
		D3DXCreateTextureFromFile(pDevice, TEXTURE_TITLE000, &m_pTexture[1]);		//	タイトルロゴ
		break;
	case CManager::MODE_TUTORIAL://	チュートリアル	---------------------------------------------------------------------
		D3DXCreateTextureFromFile(pDevice, TEXTURE_BOTTON_ENTER, &m_pTexture[0]);	//	ボタン
		D3DXCreateTextureFromFile(pDevice, TEXTURE_TUTORIAL000, &m_pTexture[2]);	//	チュートリアル
		break;
	case CManager::MODE_GAME://	ゲーム	-----------------------------------------------------------------------------
		D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME000, &m_pTexture[3]);		// LIVE
		D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME001, &m_pTexture[4]);		// 新年号
		D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME002, &m_pTexture[5]);		// 速報
		D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME003, &m_pTexture[6]);		// 返り血
		D3DXCreateTextureFromFile(pDevice, TEXTURE_GAME004, &m_pTexture[7]);		// 年号

		break;
	case CManager::MODE_RANKING://	ランキング	-------------------------------------------------------------------------
		D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING001, &m_pTexture[8]);		// 順位001
		D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING002, &m_pTexture[9]);		// 順位002
		D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING003, &m_pTexture[10]);	// 順位003
		D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING004, &m_pTexture[11]);	// 順位004
		D3DXCreateTextureFromFile(pDevice, TEXTURE_RANKING005, &m_pTexture[12]);	// 順位005


		break;
	}

	return S_OK;
}

//=============================================================================
// アンロード処理
//=============================================================================
void CUI::Unload(void)
{
	for (int nCntTex = 0; nCntTex < UI_TEXMAX; nCntTex++)
	{
		if (m_pTexture[nCntTex] != NULL)
		{// NULLの場合
		 // テクスチャの生成
			m_pTexture[nCntTex]->Release();		// 解放
			m_pTexture[nCntTex] = NULL;			// NULLへ
		}
	}
}

//=============================================================================
// 生成処理
//=============================================================================
CUI *CUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 uv, int nType)
{
	CUI *pUI = {};				// Uiポインタ

	if (pUI == NULL)
	{// NULLの場合// メモリ確保
		pUI = new CUI;
		if (pUI != NULL)
		{// NULL以外の場合
			pUI->Init(pos, size, col, uv, nType);			// 初期化処理
		}
	}
	return pUI;
}

//=============================================================================
// UI作成するモード
//=============================================================================
void CUI::CreateMode(CManager::MODE mode)
{
	m_mode = mode;	//モードの同期
	switch (mode)
	{
	case CManager::MODE_TITLE://	タイトル	------------------------------------------------------------------------
							  //	タイトルロゴ000
		CUI::Create
		(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50, 0.0f), D3DXVECTOR3(650.0f, 320.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 1);
		//	PRESS ENTER
		CUI::Create
		(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXVECTOR3(200.0f, 50.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 0);
		break;
	case CManager::MODE_TUTORIAL://	チュートリアル	---------------------------------------------------------------------
								 //	チュートリアル背景
		CUI::Create
		(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 2);
		//	PRESS ENTER
		CUI::Create
		(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200, 0.0f), D3DXVECTOR3(200.0f, 50.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 0);
		break;
	case CManager::MODE_GAME://	ゲーム	-----------------------------------------------------------------------------
							 //	LIVE
		CUI::Create
		(D3DXVECTOR3(100, 50, 0.0f), D3DXVECTOR3(90, 40, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 3);
		// 新年号
		CUI::Create
		(D3DXVECTOR3(SCREEN_WIDTH / 2, 670, 0.0f), D3DXVECTOR3(SCREEN_WIDTH / 2 - 50, 60, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 4);
		// 速報
		CUI::Create
		(D3DXVECTOR3(100, 570, 0.0f), D3DXVECTOR3(90, 40, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 5);
		// 返り血
		CUI::Create
		(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 6);
		// 年号
		CUI::Create
		(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50, 0.0f), D3DXVECTOR3(0.0f, 100, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 7);
		break;
	case CManager::MODE_RANKING://	ランキング	-------------------------------------------------------------------------
		// 順位 1位
		CUI::Create
		(D3DXVECTOR3(500.0f, 150.0f, 0.0f), D3DXVECTOR3(70.0f, 60, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 8);

		// 順位 2位
		CUI::Create
		(D3DXVECTOR3(600.0f, 280.0f, 0.0f), D3DXVECTOR3(80.0f, 60, 0.0f),
			D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 9);

		// 順位 3位
		CUI::Create
		(D3DXVECTOR3(680.0f, 400.0f, 0.0f), D3DXVECTOR3(50.0f, 40, 0.0f),
			D3DXCOLOR(0.7f, 0.5f, 0.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 10);

		// 順位 4位
		CUI::Create
		(D3DXVECTOR3(680.0f, 500.0f, 0.0f), D3DXVECTOR3(50.0f, 40, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 11);

		// 順位 5位
		CUI::Create
		(D3DXVECTOR3(680.0f, 600.0f, 0.0f), D3DXVECTOR3(50.0f, 40, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0), D3DXVECTOR2(1.0f, 1.0f), 12);
		break;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CUI::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 uv, int nType)
{
	CScene2D::SetInitAll(pos, size, col, uv);	// 初期値設定
	CScene2D::Init();							// 初期化処理
	CScene2D::BindTexture(m_pTexture[nType]);	// テクスチャ割り当て
												//	種類の設定
	m_nType = nType;
	//	昔の大きさの設定
	m_sizeOld = size;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CUI::Uninit(void)
{
	CScene2D::Uninit();						// 終了処理
}

//=============================================================================
// 更新処理
//=============================================================================
void CUI::Update(void)
{
	D3DXVECTOR3 pos = CScene2D::Getpos();	//	位置の取得
	D3DXVECTOR3 size = CScene2D::Getsize();	//	大きさの取得
	D3DXCOLOR col = CScene2D::Getcol();		//	色の取得

	m_nCounter++;							//	カウンター

											// 更新処理
	CScene2D::Update();
	switch (m_mode)
	{
	case CManager::MODE_TITLE:

		if (m_nType == 0)
		{//	プレスエンターだったら
			if (CFade::GetFade() == CFade::FADE_NONE)
			{// フェードしていない状態
				if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
				{// キーボード（ENTER）を入力したら
					m_bPressButton = true;
				}

			}
			if (m_bPressButton == false)
			{//	ボタンを押していない
				if ((m_nCounter % 40) == 0)
				{//	60秒で点滅
					m_bFlash = m_bFlash ? false : true;

				}
			}
			else if (m_bPressButton == true)
			{//	ボタンを押した
				if ((m_nCounter % 6) == 0)
				{//	60秒で点滅
					m_bFlash = m_bFlash ? false : true;

				}
			}
			if (m_bFlash == true)
			{//	サイズをなくす
				size.x = 0;
			}
			else if (m_bFlash == false)
			{//	サイズを初期の大木さんイ戻す
				size.x = 200.0f;
			}
		}
		break;
		//-----------------------------
		//	チュートリアル
		//------------------------------
	case CManager::MODE_TUTORIAL:
		if (m_nType == 0)
		{//	プレスエンターだったら
			if (CFade::GetFade() == CFade::FADE_NONE)
			{// フェードしていない状態
				if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
				{// キーボード（ENTER）を入力したら
					m_bPressButton = true;
				}

			}
			if (m_bPressButton == false)
			{//	ボタンを押していない
				if ((m_nCounter % 40) == 0)
				{//	60秒で点滅
					m_bFlash = m_bFlash ? false : true;

				}
			}
			else if (m_bPressButton == true)
			{//	ボタンを押した
				if ((m_nCounter % 6) == 0)
				{//	60秒で点滅
					m_bFlash = m_bFlash ? false : true;

				}
			}
			if (m_bFlash == true)
			{//	サイズをなくす
				size.x = 0;
			}
			else if (m_bFlash == false)
			{//	サイズを初期の大木さんイ戻す
				size.x = 200.0f;
			}
		}
		break;
		//-----------------------------
		//	ゲームモード
		//------------------------------
	case CManager::MODE_GAME:
		if (m_nType == 3)
		{//	LIVE
			if ((m_nCounter % 40) == 0)
			{//	60秒で点滅
				m_bFlash = m_bFlash ? false : true;

			}
			if (m_bFlash == true)
			{//	サイズをなくす
				size.x = 0;
			}
			else if (m_bFlash == false)
			{//	サイズを初期の大木さんイ戻す
				size.x = 90.0f;
			}
		}
		if (m_nType == 6)
		{//	返り血
			if (CPlayer::GetDisp() == true)
			{// キーボード（ENTER）を入力したら
				size.x = SCREEN_WIDTH / 2;
			}
		}
		if (m_nType == 7)
		{//	年号
			if (CPlayer::GetNengouUp() == true)
			{// キーボード（ENTER）を入力したら
				size.x = 80.0f;
			}
			else if (CPlayer::GetNengouUp() == false)
			{// キーボード（ENTER）を入力したら
				size.x = 0.0f;
			}
		}
		break;
	}

	CScene2D::Setpos(pos);				//	位置の設定
	CScene2D::Setsize(size);			//	大きさの設定
	CScene2D::Setcol(col);				//	色の設定
}

//=============================================================================
// 描画処理
//=============================================================================
void CUI::Draw(void)
{
	CScene2D::Draw();						// 描画処理
}


