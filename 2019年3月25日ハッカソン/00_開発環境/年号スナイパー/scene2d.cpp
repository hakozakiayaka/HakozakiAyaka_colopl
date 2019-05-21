//=============================================================================
//
// シーン2D処理 [scene2d.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene2d.h"		// シーン2D
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
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
CScene2D::CScene2D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// 初期値設定
	m_pTexture	= NULL;									// テクスチャ情報へのポインタ
	m_pVtxBuff	= NULL;									// マテリアル情報へのポインタ

	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_size		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 大きさ
	m_col		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色
	m_TexUV		= D3DXVECTOR2(0.0f, 0.0f);				// UV
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene2D::Init(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&m_pVtxBuff, NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f) + m_pos;
	pVtx[1].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f) + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f) + m_pos;
	pVtx[3].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f) + m_pos;

	// 1.0fで固定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// 頂点カラー設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	//	テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_TexUV.x, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, m_TexUV.y);
	pVtx[3].tex = D3DXVECTOR2(m_TexUV.x, m_TexUV.y);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScene2D::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{// NULL以外の場合
		m_pVtxBuff->Release();		// 解放
		m_pVtxBuff = NULL;			// NULLへ
	}

	// シーン開放
	CScene::SetDeath();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScene2D::Update(void)
{
	VERTEX_2D *pVtx;	// 頂点情報への

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, -m_size.y, 0.0f) + m_pos;
	pVtx[1].pos = D3DXVECTOR3(m_size.x, -m_size.y, 0.0f) + m_pos;
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, m_size.y, 0.0f) + m_pos;
	pVtx[3].pos = D3DXVECTOR3(m_size.x, m_size.y, 0.0f) + m_pos;

	// 頂点カラー設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//	テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_TexUV.x, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, m_TexUV.y);
	pVtx[3].tex = D3DXVECTOR2(m_TexUV.x, m_TexUV.y);

	// 頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void CScene2D::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
 	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//=============================================================================
//	テクスチャの設定
//=============================================================================
void CScene2D::SetTexture(D3DXVECTOR2 *pTex)
{
	//頂点情報へのポインタ
	VERTEX_2D*pVtx;
	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = pTex[0];
	pVtx[1].tex = pTex[1];
	pVtx[2].tex = pTex[2];
	pVtx[3].tex = pTex[3];

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}