//=============================================================================
//
// シーン3D処理 [scene3d.cpp]
// Author :YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3d.h"		// シーン3D
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************6
// グローバル変数
//*****************************************************************************

//*****************************************************************************
// 静的メンバ変数
//*****************************************************************************

//=============================================================================
// コンストラクタ
//=============================================================================
CScene3D::CScene3D(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pTexture	= NULL;									// テクスチャ情報へのポインタ
	m_pVtxBuff	= NULL;									// 頂点バッファ情報へのポインタ
	m_pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_size		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 大きさ
	m_col		= D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 色
	m_TexUV		= D3DXVECTOR2(0.0f, 0.0f);				// UV
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene3D::~CScene3D()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScene3D::Init(void)
{
	//	デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9	pDevice = pRenderer->GetDevice();

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4
		, D3DUSAGE_WRITEONLY
		, FVF_VERTEX_3D
		, D3DPOOL_MANAGED
		, &m_pVtxBuff
		, NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ
	// 頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, m_size.z);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, -m_size.z);

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

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScene3D::Uninit(void)
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
void CScene3D::Update(void)
{
	VERTEX_3D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(-m_size.x, m_size.y, m_size.z);
	pVtx[1].pos = D3DXVECTOR3(m_size.x, m_size.y, m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x, -m_size.y, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(m_size.x, -m_size.y, -m_size.z);

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

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void CScene3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();			// デバイス取得

	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// 計算用マトリックス

	// ライト影響受けない
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//// アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	if (m_scene3dType == SCENE3DTYPE_BILLBOARD || m_scene3dType == SCENE3DTYPE_BILLEFFECT || m_scene3dType == SCENE3DTYPE_SUBSYNTHESIS)
	{//	ビルボード　			加算合成ありビルボードエフェクト

		if (m_scene3dType == SCENE3DTYPE_BILLEFFECT)
		{
			// αブレンディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}
		else if (m_scene3dType == SCENE3DTYPE_SUBSYNTHESIS)
		{
			//// αブレンディングを減算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}
		// Zバッファへの書き込み
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	if (m_scene3dType == SCENE3DTYPE_NORMAL)
	{
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	}
	else if (m_scene3dType == SCENE3DTYPE_BILLBOARD || m_scene3dType == SCENE3DTYPE_BILLEFFECT )
	{//	ビルボード　			加算合成ありビルボードエフェクト
			// 逆行列
		m_mtxWorld._11 = mtxView._11;
		m_mtxWorld._12 = mtxView._21;
		m_mtxWorld._13 = mtxView._31;
		m_mtxWorld._21 = mtxView._12;
		m_mtxWorld._22 = mtxView._22;
		m_mtxWorld._23 = mtxView._32;
		m_mtxWorld._31 = mtxView._13;
		m_mtxWorld._32 = mtxView._23;
		m_mtxWorld._33 = mtxView._33;
	}

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットd
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	if (m_scene3dType == SCENE3DTYPE_BILLEFFECT || m_scene3dType == SCENE3DTYPE_BILLBOARD || m_scene3dType == SCENE3DTYPE_SUBSYNTHESIS)
	{//	ビルボード　			加算合成ありビルボードエフェクト
	 // Zバッファへの書き込み
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		if (m_scene3dType == SCENE3DTYPE_BILLEFFECT || m_scene3dType == SCENE3DTYPE_SUBSYNTHESIS)
		{//	ビルボード
		 // αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		}
	}
	// ライト影響受けないk
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}
