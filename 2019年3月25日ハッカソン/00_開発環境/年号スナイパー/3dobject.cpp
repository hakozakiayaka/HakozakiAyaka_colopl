//=============================================================================
//
// ビルボードオブジェクト処理 [item.cpp]
// Author : YUTARO ABE
//
//=============================================================================

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene3D.h"		// シーン3D
#include "manager.h"		// マネージャー
#include "renderer.h"		// レンダラー
#include "input.h"			// 入力
#include "camera.h"			// カメラ
#include "3dobject.h"		// 3dオブジェクト
#include "effect.h"			// エフェクト

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
LPDIRECT3DTEXTURE9 C3Dobject::m_pTexture[TYPE_MAX] = {};

//=============================================================================
// コンストラクタ
//=============================================================================
C3Dobject::C3Dobject() : CScene3D(CScene::PRIORITY_5, CScene::OBJTYPE_3DOBJECT)
{
	m_Type = {};
}

//=============================================================================
// デストラクタ
//=============================================================================
C3Dobject::~C3Dobject()
{
}

//=============================================================================
//	アイテムの生成
//=============================================================================
C3Dobject *C3Dobject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType)
{
	C3Dobject *p3Dobject = {};

	if (p3Dobject == NULL)
	{//	アイテムの生成
		p3Dobject = new C3Dobject;
		if (p3Dobject != NULL)
		{//アイテムの初期化
			p3Dobject->Init(pos, rot, size, col, TexUV, nType);
		}
	}
	return p3Dobject;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT C3Dobject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 size, D3DXCOLOR col, D3DXVECTOR2 TexUV, int nType)
{
	CScene3D::SetInitAll(pos + D3DXVECTOR3(0.0f, size.y,0.0f), rot, size, col, TexUV, CScene3D::SCENE3DTYPE_BILLBOARD);
	CScene3D::Init();
	CScene3D::BindTexture(m_pTexture[nType]);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void C3Dobject::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// 更新処理
//=============================================================================
void C3Dobject::Update(void)
{
	CScene3D::Update();
}

//=============================================================================
// 描画処理
//=============================================================================
void C3Dobject::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();			// デバイス取得

	CScene3D::Draw();

}
//=============================================================================
// テクスチャの読み込み
//=============================================================================
HRESULT C3Dobject::Load(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\tree003.png", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\tree001.png", &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\tree002.png", &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\tree002.png", &m_pTexture[3]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\kusa000.png", &m_pTexture[4]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\kusa001.png", &m_pTexture[5]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\iwa000.png", &m_pTexture[6]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\iwa001.png", &m_pTexture[7]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\3dbg\\bamboo001.png", &m_pTexture[8]);

	return S_OK;
}
//=============================================================================
// アンロード処理
//=============================================================================
void C3Dobject::Unload(void)
{
	for (int nCount = 0; nCount < TYPE_MAX; nCount++)
	{
		// テクスチャの破棄
		if (m_pTexture[nCount] != NULL)
		{
			m_pTexture[nCount]->Release();
			m_pTexture[nCount] = NULL;
		}
	}
}
