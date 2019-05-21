//=============================================================================
//
// シーンX処理 [scenex.h]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scenex.h"			// シーンX
#include "manager.h"		// マネージャー
#include "input.h"			// 入力
#include "renderer.h"		// レンダラー

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
CSceneX::CSceneX(int nPriority, CScene::OBJTYPE objType) : CScene(nPriority, objType)
{
	// 初期値設定
	m_pTexture = NULL;							// テクスチャ情報へのポインタ
	m_pMesh = NULL;								// メッシュ情報へのポインタ
	m_pBuffMat = NULL;							// マテリアル情報へのポインタ
	m_nNumMat = NULL;							// マテリアル情報の数

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_scale =D3DXVECTOR3(1.0f,1.0f, 1.0f);		// 拡大縮小
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CSceneX::~CSceneX()
{

}

//=============================================================================
// 初期化処理
//=============================================================================Z
HRESULT CSceneX::Init(void)
{
	// デバイス取得6
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファのポインタ

	// 頂点数の取得
	nNumVtx = m_pMesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{// カウント
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		// 頂点座標の代入

		// 最大を求める
		if (vtx.x > m_vtxMax.x)
		{
			m_vtxMax.x = vtx.x;
		}
		if (vtx.y > m_vtxMax.y)
		{
			m_vtxMax.y = vtx.y;
		}
		if (vtx.z > m_vtxMax.z)
		{
			m_vtxMax.z = vtx.z;
		}

		// 最小を求める
		if (vtx.x < m_vtxMin.x)
		{
			m_vtxMin.x = vtx.x;
		}
		if (vtx.y < m_vtxMin.y)
		{
			m_vtxMin.y = vtx.y;
		}
		if (vtx.z < m_vtxMin.z)
		{
			m_vtxMin.z = vtx.z;
		}

		pVtxBuff += sizeFVF;	// サイズ分ポインタを進める
	}

	// 頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();

	//	大きさに合わせて当たりの調整をする
	m_vtxMax = D3DXVECTOR3(m_vtxMax.x *m_scale.x, m_vtxMax.y *m_scale.y, m_vtxMax.z *m_scale.z);
	m_vtxMin = D3DXVECTOR3(m_vtxMin.x *m_scale.x, m_vtxMin.y *m_scale.y, m_vtxMin.z *m_scale.z);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CSceneX::Uninit(void)
{
	// シーン開放
	CScene::SetDeath();
}

//=============================================================================
// 更新処理
//=============================================================================
void CSceneX::Update(void)
{


}

//=============================================================================
// 描画処理
//=============================================================================
void CSceneX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイス取得
	D3DXMATRIX mtxRot, mtxTrans, matScale;					// 計算用マトリックス
	D3DMATERIAL9 matDef;									// 現在のマテリアル保存用
	D3DXMATERIAL *pmat;										// マテリアルデータへのポインタ
															//// アルファテストの設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//	拡大縮小行列の作成
	D3DXMatrixScaling(&matScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &matScale);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pmat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{// カウント


		//	発光
		//pmat[nCntMat].MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);

		// マテリアルの設定
		pDevice->SetMaterial(&pmat[nCntMat].MatD3D);

		// テクスチャ設定
		pDevice->SetTexture(0, m_pTexture[nCntMat]);

		// オブジェクト(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);

	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}