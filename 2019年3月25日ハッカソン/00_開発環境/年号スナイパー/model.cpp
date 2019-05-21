//=============================================================================
//
// モデル処理 [model.cpp]
// Author : 箱﨑彩花
//
//=============================================================================
// ファイル読み込みで必要
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>				//インクルードファイル
#include <string.h>

#include "manager.h"			// マネージャー
#include "renderer.h"			// レンダリング
#include "input.h"				// 入力
#include "scene.h"				// scene
#include "model.h"				// モデル
#include "DebugProc.h"		// デバッグ表示

//*****************************************************************************
// 静的メンバ変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9 CModel::m_pTexture = NULL;		// テクスチャのポインタ
char CModel::m_acFileName[MAX_MODEL][MAX_CHAR] = {};		// 読み込むモデルの名前

															//=============================================================================
															// コンストラクタ
															//=============================================================================
CModel::CModel()
{
	m_apMesh = NULL;
	m_apBuffMat = NULL;
	m_aNumMat = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CModel::~CModel()
{
}

#if 1
//=============================================================================
// パーツ読み込み
//=============================================================================
HRESULT CModel::Load(LPCSTR Memory)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;											//デバイスの取得
	CRenderer * pRenderer = CManager::GetRenderer();						//レンダリングクラスの呼び出し

	if (pRenderer != NULL)
	{
		pDevice = pRenderer->GetDevice();
	}

	// Xファイルの読み込み
	D3DXLoadMeshFromX(Memory, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_apBuffMat, NULL, &m_aNumMat, &m_apMesh);

	return S_OK;
}
#endif

//=============================================================================
// パーツの破棄
//=============================================================================
void CModel::Unload(void)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		m_acFileName[nCntModel][0] = NULL;
	}
}

//=============================================================================
// モデルの生成
//=============================================================================
CModel * CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumParts, MODELTYPE type)
{
	CModel * pModel = NULL;

	if (pModel == NULL)
	{	// モデルの動的確保
		pModel = new CModel;
	}
	if (pModel != NULL)
	{
		pModel->m_nNumParts = nNumParts;
		pModel->m_type = type;
		pModel->Init();				// 初期化
		pModel->m_pos = pos;	// 位置
		pModel->m_rot = rot;		// 向き
	}

	return pModel;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CModel::Init(void)
{	// 値の初期化はしまい！（パーツが全て初期化されちゃう）

	LPDIRECT3DDEVICE9 pDevice = NULL;									// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();						// レンダリングクラスの呼び出し

	if (pRenderer != NULL)
	{	//Rendererクラスの取得
		pDevice = pRenderer->GetDevice();
	}


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CModel::Uninit(void)
{

	// メッシュの開放
	if (m_apMesh != NULL)
	{
		m_apMesh->Release();
		m_apMesh = NULL;
	}

	// マテリアルの開放
	if (m_apBuffMat != NULL)
	{
		m_apBuffMat->Release();
		m_apBuffMat = NULL;
	}

	if (m_aNumMat != NULL)
	{
		m_aNumMat = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CModel::Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void CModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;										// デバイスの取得
	CRenderer *pRenderer = CManager::GetRenderer();						// レンダリングクラスの呼び出し

	if (pRenderer != NULL)
	{	//Rendererクラスの取得
		pDevice = pRenderer->GetDevice();
	}

	D3DXMATRIX mtxRot, mtxTrans, mtxParent;						//計算用マトリックス
	D3DMATERIAL9 matDef;										//現在のマトリックス保存用
	D3DXMATERIAL * pMat;										//マテリアルデータへのポインタ

	D3DXMATRIX mtxShadow;										//　計算用マトリックス
	D3DXPLANE planeField;										// プレーンフィールド
	D3DXVECTOR4 vecLight;										//　ライトのベクトル
	D3DXVECTOR3 pos, normal;									// 位置、法線

																// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動（位置）を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 親のマトリックスと掛け合わせる
	if (m_pParent != NULL)
	{
		mtxParent = m_pParent->GetMtxWorld();			// CModelのマトリックスを取得
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// プレイヤーとモデルのマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	if (m_apMesh != NULL)
	{
		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)m_apBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_aNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			pDevice->SetTexture(0, m_pTexture);

			// モデル(パーツ)の描画
			m_apMesh->DrawSubset(nCntMat);
		}
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// テクスチャの割り当て
//=============================================================================
void CModel::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;
}