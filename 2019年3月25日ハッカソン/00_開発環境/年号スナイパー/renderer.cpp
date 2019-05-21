//=============================================================================
//
// レンダラー処理 [renderer.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "renderer.h"		// レンダラー
#include "manager.h"		// マネージャー
#include "scene.h"			// シーン
#include "input.h"			// 入力

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

//=============================================================================
// コンストラクタ
//=============================================================================
CRenderer::CRenderer()
{
	m_bWire = false;		//	ワイヤーフレーム
	m_pD3D = NULL;			//	オブジェクト
	m_pD3DDevice = NULL;			//	デバイス
}

//=============================================================================
// デストラクタ
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;		//	ディスプレイモード
	D3DDISPLAYMODE d3ddm;				//	プレゼンテーションパラメータ

										// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

																// デバイスの生成
																// ディスプレイアダプタを表すためのデバイスを作成
																// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード有効
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

																			// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

																			// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理を設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// 拡大用(バイリニア・フィルタリング)
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// 縮小用(バイリニア・フィルタリング)

	m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// 拡大用(バイリニア・フィルタリング)
	m_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// 縮小用(バイリニア・フィルタリング)

																			// ステンシルバッファの設定
																			m_pD3DDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);
																			m_pD3DDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
																			m_pD3DDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
																			m_pD3DDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);


																			//D3DCAPS9 caps;
																			//ZeroMemory(&caps, sizeof(D3DCAPS9));    //	初期化
																			//m_pD3DDevice->GetDeviceCaps(&caps);

																			////	ピクセルフォグが使えるかどうか確認
																			//if ((caps.RasterCaps & D3DPRASTERCAPS_FOGRANGE) == 0)
																			//{
																			//	return FALSE;
																			//}

																			////	フォグの設定
																			//FLOAT StartPos = 1300;	//	開始位置
																			//FLOAT EndPos = 1800;	//	終了位置
																			//m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);								//	フォグ：ON
																			//m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_RGBA(0, 0, 0, 150));			//	白色で不透明
																			//m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);						//	頂点モード
																			//m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);					//	テーブルモード
																			//m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&StartPos));					//	開始位置
																			//m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&EndPos));						//	終了位置

																			//	ワイヤーフレーム
	m_bWire = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRenderer::Uninit(void)
{
	CScene::ReleaseAll();

	// デバイスの破棄
	if (m_pD3DDevice != NULL)
	{// NULL以外の場合
		m_pD3DDevice->Release();	// 解放
		m_pD3DDevice = NULL;		// NULLへ
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{// NULL以外の場合
		m_pD3D->Release();			// 解放
		m_pD3D = NULL;				// NULLへ
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CRenderer::Update(void)
{

	CInputKeyboard *pInputKeyboad = CManager::GetInputKeyboard();

	if (pInputKeyboad->GetTrigger(DIK_F2) == true)
	{//	バイナリのデータの書き込み
		m_bWire = m_bWire ? false : true;
	}

	CScene::UpadteAll();			// 更新処理
}

//=============================================================================
// 描画処理
//=============================================================================
void CRenderer::Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	//m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(100, 149, 237, 0), 1.0f, 0);
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		CScene::DrawAll();			// 描画処理

		CManager::m_CDebugProcLeft->Draw();
		CManager::m_CDebugProcRight->Draw();
#ifdef _DEBUG
		if (m_bWire == true)
		{//	ワイヤー状に設定する
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); // ワイヤフレームモード
			CManager::m_CDebugProcLeft->Print("F2 :ワイヤーフレーム　：ON");
		}
		else if (m_bWire == false)
		{//	ワイヤー状に設定しない
			m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	// ワイヤフレームモード
			CManager::m_CDebugProcLeft->Print("F2 :ワイヤーフレーム　：OFF");
		}
#endif
		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}