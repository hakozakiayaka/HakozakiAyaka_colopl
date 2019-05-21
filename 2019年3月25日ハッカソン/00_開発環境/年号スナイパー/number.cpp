//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆
//数字・ナンバー処理 [number.cpp]
//Author : Kobayashi_Sho-hei(小林 将兵)
//◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆　◆

#include "number.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;
//=============================================================================
//+ 数字・ナンバーのコンストラクタ
//=============================================================================
CNumber::CNumber()
{
	//! <値をクリアな状態にする>
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ
	m_SizeX = NULL;
	m_SizeY = NULL;


	//m_pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);//表示するポリゴンの位


}
//=============================================================================
//+ 数字・ナンバーのデストラクタ
//=============================================================================
CNumber::~CNumber()
{

}
//=============================================================================
//+ 数字・ナンバーのTexの必要分読み込み
//=============================================================================
HRESULT CNumber::Load(void)
{
	//デバイス情報の取得↓
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TIME_TEX_NAME01, &m_pTexture);

	return S_OK;
}
//=============================================================================
//+ 数字・ナンバーのTexを他の処理による削除をさせない
//=============================================================================
void CNumber::UnLoad(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
//+ 数字・ナンバーの生成処理
//=============================================================================
CNumber *CNumber::Create(void)
{
	CNumber *pNumber = NULL;

	if (pNumber == NULL)
	{
		pNumber = new CNumber;
		pNumber->Init();
	}
	return pNumber;
}
//=============================================================================
//+ 数字・ナンバーの初期化処理
//=============================================================================
HRESULT CNumber::Init(void)
{
	//デバイス情報の取得↓
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//頂点情報の作成
	VERTEX_2D *pVtx;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//ポリゴンの位置を設定
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);

	//テクスチャの生成
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//rhw
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラー情報
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}
//=============================================================================
//+ 数字・ナンバーの終了処理
//=============================================================================
void CNumber::Uninit(void)
{

	//テクスチャの破棄
	//! 各テクスチャを扱う.cppに書き込んでいる

	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	delete this;

}
//=============================================================================
//+ 数字・ナンバーの更新処理
//=============================================================================
void CNumber::Update(void)
{

}
//=============================================================================
//+ 数字・ナンバーの描画処理
//=============================================================================
void CNumber::Draw(void)
{
	//デバイス情報の取得↓
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//=============================================================================
//+ 
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	//頂点情報の作成
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);


	//テクスチャの生成
	pVtx[0].tex = D3DXVECTOR2(0.0f + nNumber * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + nNumber * 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + nNumber * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + nNumber * 0.1f, 1.0f);


	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//表示位置・間隔・桁数
//=============================================================================
void CNumber::SetPos(D3DXVECTOR3 pos, int nSpace, int nCnt)
{
	//頂点情報の作成
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//ポリゴンの位置を設定
	pVtx[0].pos = D3DXVECTOR3(pos.x - m_SizeX + (nSpace*nCnt), pos.y - m_SizeY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + m_SizeX + (nSpace * nCnt), pos.y - m_SizeY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - m_SizeX + (nSpace * nCnt), pos.y + m_SizeY, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + m_SizeX + (nSpace * nCnt), pos.y + m_SizeY, 0.0f);


	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
//サイズの調整
//=============================================================================
void CNumber::SetSize(float X, float Y)
{
	m_SizeX = X;
	m_SizeY = Y;
}